#ifndef SIM_QUEUE_IO_H
#define SIM_QUEUE_IO_H

#include <fstream> 
#include <thread>
#include "buffer_fs.hpp"

namespace sim{
    namespace io{

        //-----------------------------------------------------------------------//
        // Time an input thread waits on an empty queue until it rechecks if the 
        // producer thread is done (eof == true)
        //-----------------------------------------------------------------------//
        constexpr size_t DEAD_TIME_MILLIS = 10;
        constexpr size_t CHUNK_SIZE = 1<<14;


        //-----------------------------------------------------------------------//
        // Template for lock-free queue based input.
        //-----------------------------------------------------------------------//
        template <typename T> class BufferInput { 

            public:

                //---------------------------------------------------------------//
                BufferInput<T>(std::string id) : 
                    queue_handle(open<std::vector<T>>(id)) {
                        current = current_chunk.begin();
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                BufferInput<T> (BufferInput<T> &other) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                BufferInput<T> &operator=(BufferInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                BufferInput<T>(BufferInput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                BufferInput<T> &operator=(BufferInput<T> &&other) = default;

                //---------------------------------------------------------------//
                bool get(T &target) {
                    bool valid = true; // true if value set to target is valid
                    if (current == current_chunk.end()){
                        valid = get_next();
                    }
                    target = *current++;
                    return valid;
                }

                //---------------------------------------------------------------//
                bool get_chunk(std::vector<T> &target){
                    bool valid = get_next(); // true if a new (valid) chunk is provided
                    target = std::move(current_chunk);
                    return valid;
                }

                //---------------------------------------------------------------//
                T peek() const {
                    return *current;
                }

            private:

                //---------------------------------------------------------------//
                // Get the next chunk to the current member
                //---------------------------------------------------------------//
                bool get_next(){
                    std::vector<T> next_chunk;
                    do {
                        if (queue_handle.queue->wait_dequeue_timed(next_chunk,
                                    std::chrono::milliseconds(DEAD_TIME_MILLIS))){
                            current_chunk = std::move(next_chunk);
                            current = current_chunk.begin();
                            return true;
                        } 
                    }while(!queue_handle.eof->load());
                    return false;
                }

                //---------------------------------------------------------------//
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> current_chunk{};
                typename std::vector<T>::iterator current;

        };

        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        
        //-----------------------------------------------------------------------//
        // Template for lock-free queue based output.
        //-----------------------------------------------------------------------//
        template <typename T> class BufferOutput { 

            public:

                //---------------------------------------------------------------//
                BufferOutput<T>(std::string id) :
                    queue_handle(open<std::vector<T>>(id)) { 
                        make_new_chunk();
                    }

                //---------------------------------------------------------------//
                ~BufferOutput<T>(){
                    // notify the user that the output is done.
                    push_chunk();
                    queue_handle.eof->store(true);
                }

                //-Copy-ctor:-DELETED--------------------------------------------//
                BufferOutput<T>(BufferOutput<T> &source) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                BufferOutput<T> &operator=(BufferOutput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                BufferOutput<T>(BufferOutput<T> &&other)  = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                BufferOutput<T> &operator=(BufferOutput<T> &&other) = default;

                //---------------------------------------------------------------//
                void put(T &item) {
                    chunk.push_back(item);
                    if (chunk.size() >= CHUNK_SIZE){
                        push_chunk();
                        make_new_chunk();
                    }
                }

                //---------------------------------------------------------------//
                void put_chunk(std::vector<T> c){
                    push_chunk(); // commit current chunk
                    chunk = std::move(c);
                }

            private:

                //---------------------------------------------------------------//
                void make_new_chunk(){
                    chunk = std::vector<T>();
                    chunk.reserve(CHUNK_SIZE);
                }

                void push_chunk(){
                    if (chunk.size() > 0) {
                        queue_handle.queue->enqueue(std::move(chunk));
                    }
                }
                
                //---------------------------------------------------------------//
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> chunk;
                

        };

        //-----------------------------------------------------------------------//
        template <typename T> void buffer2file(std::string buffer_id, std::string filename = ""){
            if (filename == "") filename = buffer_id;
            auto queue = BufferInput<T>(buffer_id);
            auto os = std::ofstream(filename, std::ofstream::binary);
            std::vector<T> chunk{};
            while (queue.get_chunk(chunk)){
                char const *data = reinterpret_cast<char const *>(chunk.data());
                os.write(data, chunk.size()*sizeof(T));
            }
        }

        //-----------------------------------------------------------------------//
        template <typename T> std::thread buffer2file_thread(std::string buffer_id, std::string filename = ""){
            std::thread thr{ [&] () { buffer2file<T>(buffer_id, filename); } };
            return thr;
        }

        //-----------------------------------------------------------------------//
        template <typename T> void file2buffer(std::string filename, std::string buffer_id = ""){
            if (buffer_id == "") buffer_id = filename;
            auto queue = BufferOutput<T>(buffer_id);
            auto is = std::ifstream(filename, std::ifstream::binary);
            while (!is.eof()){
                std::vector<T> chunk{CHUNK_SIZE};
                chunk.resize(CHUNK_SIZE);
                is.read(reinterpret_cast<char *>(chunk.data()), CHUNK_SIZE*sizeof(T));
                chunk.resize(is.gcount()/sizeof(T));
                queue.put_chunk(chunk);
            }
        }

        //-----------------------------------------------------------------------//
        template <typename T> std::thread file2buffer_thread(std::string filename, std::string buffer_id = ""){
            std::thread thr{ [&] () { file2buffer<T>(filename, buffer_id); } };
            return thr;
        }


    }
}

#endif
