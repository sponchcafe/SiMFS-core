#pragma once

#include <fstream> 
#include <thread>
#include "buffer_fs.hpp"

namespace sim{
    namespace io{

        //-----------------------------------------------------------------------//
        // Time an input thread waits on an empty queue until it rechecks if the 
        // producer thread is done (eof == true)
        //-----------------------------------------------------------------------//
        constexpr size_t CHUNK_SIZE_BYTES = 1<<14; // 16kB chunks
        constexpr unsigned int LOW_WATERMARK = 64;
        constexpr unsigned int HIGH_WATERMARK = 1024;
        constexpr unsigned int BASE_DELAY_NS = 1;
        constexpr unsigned int TOP_DELAY_NS = 1024*1024;


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

                //---------------------------------------------------------------//
                ~BufferInput<T>() {
                    io::close<std::vector<T>>(buffer_id);
                }

                //---------------------------------------------------------------//
                bool get(T &target) {
                    if (done) return false;
                    target = *current++;
                    if (current == current_chunk.end()) get_next_chunk();
                    return true;
                }
        

                //---------------------------------------------------------------//
                bool get_chunk(std::vector<T> &target){
                    if (done) return false;
                    target = std::move(current_chunk);
                    get_next_chunk();
                    return true; // true if a new (valid) chunk is provided
                }

                //---------------------------------------------------------------//
                T peek() const {
                    if (done) return T{};
                    return *current;
                }

                //-Compare-input-order-by-current-value--------------------------//
                bool operator< (BufferInput<T> const &rhs) const {
                    return peek() < rhs.peek();
                }

                //---------------------------------------------------------------//
                bool is_done(){
                    return done;
                }

            private:

                //---------------------------------------------------------------//
                // Get the next chunk to the current member
                //---------------------------------------------------------------//
                void get_next_chunk(){
                    queue_handle.queue->wait_dequeue(current_chunk);
                    if (current_chunk.size() == 0){
                        done = true;
                    }
                    else{
                        current = current_chunk.begin();
                    }
                }

                //---------------------------------------------------------------//
                std::string const buffer_id;
                queue_handle_t<std::vector<T>> &queue_handle;
                std::vector<T> current_chunk{};
                static size_t const chunk_size = CHUNK_SIZE_BYTES/sizeof(T);
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
                    push_chunk();
                    push_empty_end_chunk(); // end of stream
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
                    if (queue_handle.queue->size_approx() == 0 || chunk.size() >= chunk_size){
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
                void apply_delay(){

                    size_t s = queue_handle.queue->size_approx();

                    if (s < LOW_WATERMARK) {
                        delay = BASE_DELAY_NS;
                        return;
                    }

                    else if(s >= HIGH_WATERMARK) {
                        delay *= 2;
                    }
                    else {
                        delay = delay > TOP_DELAY_NS ? TOP_DELAY_NS : delay;
                        delay = delay/2 < BASE_DELAY_NS ? BASE_DELAY_NS : delay/2;
                    }
                    std::this_thread::sleep_for(std::chrono::nanoseconds(delay));

                }

                void make_new_chunk(size_t const size=chunk_size){
                    chunk = std::vector<T>();
                    chunk.reserve(size);
                }

                void push_chunk(){
                    apply_delay();
                    if (chunk.size() > 0) {
                        queue_handle.queue->enqueue(std::move(chunk));
                    }
                }

                void push_empty_end_chunk(){
                    make_new_chunk(0);
                    queue_handle.queue->enqueue(std::move(chunk));
                }

                
                //---------------------------------------------------------------//
                queue_handle_t<std::vector<T>> &queue_handle;
                static size_t const chunk_size = CHUNK_SIZE_BYTES/sizeof(T);
                std::vector<T> chunk;
                unsigned int delay = BASE_DELAY_NS;
                

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
                chunk.resize(CHUNK_SIZE_BYTES/sizeof(T));
                is.read(reinterpret_cast<char *>(chunk.data()), CHUNK_SIZE_BYTES);
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

