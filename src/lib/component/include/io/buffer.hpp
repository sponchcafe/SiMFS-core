#pragma once

#include <fstream> 
#include <thread>
#include <cmath>
#include "buffer_fs.hpp"

namespace sim{
    namespace io{

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

                size_t get_size_approx() const {
                    return queue_handle.queue->size_approx();
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
                typename std::vector<T>::iterator current;
                bool done = false;

        };

        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        //-----------------------------------------------------------------------//
        
        //-----------------------------------------------------------------------//
        // Control parameters for buffer delays
        //-----------------------------------------------------------------------//
        
        extern bool SIMFS_BUFFER_CONTROL;
        extern unsigned long int SIMFS_DELAY_NS;
        extern unsigned long int SIMFS_CHUNK_SIZE_BYTES;
        extern unsigned long int SIMFS_BUFFER_SIZE_BYTES;

        //-----------------------------------------------------------------------//
        // Template for lock-free queue based output.
        //-----------------------------------------------------------------------//
        template <typename T> class BufferOutput { 

            public:

                //---------------------------------------------------------------//
                BufferOutput<T>(std::string id) :
                    queue_handle(open<std::vector<T>>(id)) { 
                        make_new_chunk(chunk_size_n);
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
                    if (queue_handle.queue->size_approx() == 0 || chunk.size() >= chunk_size_n){
                        push_chunk();
                        make_new_chunk(chunk_size_n);
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

                    if (s < max_chunks_n/2) {
                        // std::cerr << " -- no delay\n";
                        return;
                    }


                    unsigned long int delay = calc_delay_ns(s);

                    /*
                    std::cerr << "log2(D): " << log2_delay_ns << '\n';
                    std::cerr << "d: " << delay << '\n';
                    std::cerr << "D: " << delay_ns << '\n';
                    std::cerr << "s: " << s << '\n';
                    std::cerr << "M: " << max_chunks_n << '\n';
                    std::cerr << "f: " << fill_ratio << '\n';
                    */

                    std::this_thread::sleep_for(std::chrono::nanoseconds(delay));

                }

                unsigned long int calc_delay_ns(size_t chunk_count){
                    double fill_ratio = ((double) chunk_count) / max_chunks_n;
                    return (unsigned long int) (delay_ns * pow(2, -2*(1-fill_ratio)*log2_delay_ns));
                }

                void make_new_chunk(size_t size){
                    chunk = std::vector<T>();
                    chunk.reserve(size);
                }

                void push_chunk(){
                    if (SIMFS_BUFFER_CONTROL) apply_delay();
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
                std::vector<T> chunk;

                //-Buffer-control------------------------------------------------//
                size_t chunk_size_n = SIMFS_CHUNK_SIZE_BYTES / sizeof(T);
                size_t max_chunks_n = SIMFS_BUFFER_SIZE_BYTES / SIMFS_CHUNK_SIZE_BYTES;
                unsigned long int delay_ns = SIMFS_DELAY_NS;
                double log2_delay_ns = log2((double) SIMFS_DELAY_NS);

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
                chunk.resize(SIMFS_CHUNK_SIZE_BYTES/sizeof(T));
                is.read(reinterpret_cast<char *>(chunk.data()), SIMFS_CHUNK_SIZE_BYTES);
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

