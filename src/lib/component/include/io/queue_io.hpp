#ifndef SIM_QUEUE_IO_H
#define SIM_QUEUE_IO_H

#include <fstream> 
#include <thread>
#include "queue_fs.hpp"
#include "io/base_io.hpp"

using namespace sim::comp;

namespace sim{
    namespace queue_io{

        //-----------------------------------------------------------------------//
        // Time an input thread waits on an empty queue until it rechecks if the 
        // producer thread is done (eof == true)
        //-----------------------------------------------------------------------//
        constexpr size_t DEAD_TIME_MILLIS = 10;
        constexpr size_t CHUNK_SIZE = 1<<14;


        //-----------------------------------------------------------------------//
        // Template for lock-free queue based input.
        //-----------------------------------------------------------------------//
        template <typename T> class QueueInput : public Input<T>{

            public:

                //---------------------------------------------------------------//
                QueueInput<T>(std::string id) : 
                    Input<T>(id),
                    queue_handle(open<std::vector<T>>(id)) {
                    }

                //-Copy-ctor:-DELETED--------------------------------------------//
                QueueInput<T> (QueueInput<T> &other) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                QueueInput<T> &operator=(QueueInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                QueueInput<T>(QueueInput<T> &&other) = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                QueueInput<T> &operator=(QueueInput<T> &&other) = default;

                //---------------------------------------------------------------//
                bool get(T &target) override {
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
                T peek() const override {
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
        template <typename T> class QueueOutput : public Output<T>{

            public:

                //---------------------------------------------------------------//
                QueueOutput<T>(std::string id) :
                    Output<T>(id),
                    queue_handle(open<std::vector<T>>(id)) { 
                        make_new_chunk();
                    }

                //---------------------------------------------------------------//
                ~QueueOutput<T>(){
                    // notify the user that the output is done.
                    if (chunk.size() > 0) {
                        push_chunk();
                    }
                    queue_handle.eof->store(true);
                }

                //-Copy-ctor:-DELETED--------------------------------------------//
                QueueOutput<T>(QueueOutput<T> &source) = delete;

                //-Copy-assgin:-DELETED------------------------------------------//
                QueueOutput<T> &operator=(QueueOutput<T> &other) = delete;

                //-Move-ctor:-DEFAULT--------------------------------------------//
                QueueOutput<T>(QueueOutput<T> &&other)  = default;

                //-Move-assign:-DEFAULT------------------------------------------//
                QueueOutput<T> &operator=(QueueOutput<T> &&other) = default;

                //---------------------------------------------------------------//
                void put(T &item) override {
                    chunk.push_back(item);
                    if (chunk.size() >= CHUNK_SIZE){
                        push_chunk();
                        make_new_chunk();
                    }
                }

                //---------------------------------------------------------------//
                void put_chunk(std::vector<T> &c){
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
                    queue_handle.queue->enqueue(std::move(chunk));
                }
                
                //---------------------------------------------------------------//
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> chunk;
                

        };

        //-----------------------------------------------------------------------//
        template <typename T> void queue_to_file(std::string id){
            auto queue = queue_io::QueueInput<T>(id);
            auto os = std::ofstream(id, std::ofstream::binary);
            std::vector<T> chunk;
            while (queue.get_chunk(chunk)){
                char const *data = reinterpret_cast<char const *>(chunk.data());
                os.write(data, chunk.size()*sizeof(T));
            }
        }

        //-----------------------------------------------------------------------//
        template <typename T> void file_to_queue(std::string id){
            auto queue = queue_io::QueueOutput<T>(id);
            auto is = std::ifstream(id, std::ifstream::binary);
            std::vector<T> chunk;
            while (!is.eof()){
                chunk.reserve(CHUNK_SIZE);
                is.read(reinterpret_cast<char *>(chunk.data()), CHUNK_SIZE*sizeof(T));
                chunk.resize(is.gcount()/sizeof(T));
                queue.push_chunk(std::move(chunk));
            }
        }

        //-----------------------------------------------------------------------//
        template <typename T> std::thread file_to_queue_thread(std::string id){
            std::thread thr{ [&] () { file_to_queue<T>(id); } };
            return thr;
        }

        //-----------------------------------------------------------------------//
        template <typename T> std::thread queue_to_file_thread(std::string id){
            std::thread thr{ [&] () { queue_to_file<T>(id); } };
            return thr;
        }

    }
}

#endif
