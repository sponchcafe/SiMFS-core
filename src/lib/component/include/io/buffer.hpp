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
        constexpr unsigned int DEAD_TIME_NANOS_MIN = 1<<8; // 32 ns min delay
        constexpr unsigned int DEAD_TIME_NANOS_MAX = 1<<30; // 1 s max delay
        constexpr size_t CHUNK_SIZE = 1<<12; // 8096 elements per chunk


        //-----------------------------------------------------------------------//
        // Template for lock-free queue based input.
        //-----------------------------------------------------------------------//
        template <typename T> class BufferInput { 

            public:

                //---------------------------------------------------------------//
                BufferInput<T>(std::string id) : 
                    buffer_id(id),
                    queue_handle(open<std::vector<T>>(id)) {
                        get_next_chunk();
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                BufferInput<T> (BufferInput<T> &other) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                BufferInput<T> &operator=(BufferInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                BufferInput<T>(BufferInput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                BufferInput<T> &operator=(BufferInput<T> &&other) = default;

                ~BufferInput<T>() {
                    io::close<std::vector<T>>(buffer_id);
                }

                //---------------------------------------------------------------//
                bool get(T &target) {
                    if (current == current_chunk.end()){
                        if(!get_next_chunk()){
                            return false;
                        }
                    }
                    target = *current++;
                    return true;
                }
        

                //---------------------------------------------------------------//
                bool get_chunk(std::vector<T> &target){
                    if (!done){
                        target = std::move(current_chunk);
                    } else {
                        return false;
                    }
                    done = !get_next_chunk();
                    return true; // true if a new (valid) chunk is provided
                }

                //---------------------------------------------------------------//
                T peek() const {
                    return *current;
                }

                //-Compare-input-order-by-current-value--------------------------//
                bool operator< (BufferInput<T> const &rhs) const {
                    return peek() < rhs.peek();
                }

            private:

                //---------------------------------------------------------------//
                // Get the next chunk to the current member
                //---------------------------------------------------------------//
                bool get_next_chunk(){
                    std::vector<T> next_chunk;
                    do {
                        if (queue_handle.queue->wait_dequeue_timed(next_chunk,
                                    std::chrono::microseconds(deadtime))){
                            current_chunk = std::move(next_chunk);
                            current = current_chunk.begin();
                            half_dead_time(); 
                            return true;
                        } 
                        else{
                            double_dead_time();
                        }
                    }while(!queue_handle.eof->load());
                    current = current_chunk.begin();
                    return false;
                }

                //---------------------------------------------------------------//
                void double_dead_time(){
                    if (deadtime >= DEAD_TIME_NANOS_MAX) {
                        deadtime =  DEAD_TIME_NANOS_MAX;
                    }
                    else{
                        deadtime<<=2;
                    }
                }

                //---------------------------------------------------------------//
                void half_dead_time(){
                    if (deadtime <= DEAD_TIME_NANOS_MIN) {
                        deadtime =  DEAD_TIME_NANOS_MIN;
                    }
                    else{
                        deadtime>>=2;
                    }
                }

                //---------------------------------------------------------------//
                std::string const buffer_id;
                unsigned long int deadtime = DEAD_TIME_NANOS_MIN;
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> current_chunk{};
                typename std::vector<T>::iterator current;
                bool done = false;

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
                    else{
                    }
                }
                
                //---------------------------------------------------------------//
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> chunk;
                

        };

        //-----------------------------------------------------------------------//
        template <typename T> void buffer2file(
                std::string buffer_id, 
                std::string filename = "")
        {

            if (filename == "") filename = buffer_id;
            auto queue = BufferInput<T>(buffer_id);
            auto os = std::ofstream(filename, std::ofstream::binary);

            if (!os.good()){
                os.close();
                std::cerr << "Failed to open " << filename << "\n";
                return;
            }

            std::vector<T> chunk{};
            while (queue.get_chunk(chunk)){
                char const *data = reinterpret_cast<char const *>(chunk.data());
                os.write(data, chunk.size()*sizeof(T));
            }
            os.close();

        }

        //-----------------------------------------------------------------------//
        // Capture by copy (=) essential for safe thread initialization
        //-----------------------------------------------------------------------//
        template <typename T> std::thread buffer2file_thread(
                std::string buffer_id, 
                std::string filename = ""){

            return std::thread{ 
                [=] () { 
                    buffer2file<T>(buffer_id, filename); 
                }
            };
        }

        //-----------------------------------------------------------------------//
        template <typename T> void file2buffer(std::string filename, std::string buffer_id = ""){

            if (buffer_id == "") buffer_id = filename;
            auto queue = BufferOutput<T>(buffer_id);
            auto is = std::ifstream(filename, std::ifstream::binary);

            if (!is.good()){
                is.close();
                std::cerr << "Failed to open " << filename << "\n";
                return;
            }

            while (!is.eof()){
                std::vector<T> chunk{};
                chunk.resize(CHUNK_SIZE);
                is.read(reinterpret_cast<char *>(chunk.data()), CHUNK_SIZE*sizeof(T));
                chunk.resize(is.gcount()/sizeof(T));
                queue.put_chunk(chunk);
            }
            is.close();

        }

        //-----------------------------------------------------------------------//
        // Capture by copy (=) essential for safe thread initialization
        //-----------------------------------------------------------------------//
        template <typename T> std::thread file2buffer_thread(
                std::string filename, 
                std::string buffer_id = "")
        {
            return std::thread{ 
                [=] () { 
                    file2buffer<T>(filename, buffer_id); 
                }
            };
        }


    }
}

#endif
