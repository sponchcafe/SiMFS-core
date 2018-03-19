#ifndef SIM_CMDUTILS_IO_H
#define SIM_CMDUTILS_IO_H

#include <iostream>
#include <fstream>
#include <string>

#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable

#include <unistd.h> // getpid

namespace sim{
    namespace io{

        //-Definitions-------------------------------------------------------//
        size_t const DEFAULT_SIZE = 1024*1024;
        std::string const EMPTY_FILENAME = "";


        //-Deprecated-------------------------------------------------------//
        template <typename T> std::ostream & write_binary(
                std::ostream& os, T const & t){
            char const * out_buffer = reinterpret_cast<char const *>(&t);
            return os.write(out_buffer, sizeof(T));
        }


        //-Deprecated-------------------------------------------------------//
        template <typename T> std::istream & read_binary(
                std::istream &is, T& t){
            char *in_buffer = reinterpret_cast<char *>(&t);
            is.read(in_buffer, sizeof(t));
            if (!is) is.setstate(std::ios_base::failbit);
            return is;
        }

        //-------------------------------------------------------------------//
        template <typename T> class Output{

            public:

                //-----------------------------------------------------------//
                Output<T>() : Output<T>(EMPTY_FILENAME, DEFAULT_SIZE)
                {}

                //-----------------------------------------------------------//
                Output<T>(size_t s) : Output<T>(EMPTY_FILENAME, s)
                {}
            
                //-----------------------------------------------------------//
                Output<T>(std::string filename, size_t s=DEFAULT_SIZE) 
                    : n_bytes(s)
                {
                    if (filename != EMPTY_FILENAME){
                        // filename specified, open file and set to output
                        outfile = std::ofstream(filename, 
                                std::ofstream::binary);
                        output = &outfile;
                    }
                    else{
                        // no filename, set output to stdout
                        output = &std::cout;
                    }
                    init();
                }

                //-----------------------------------------------------------//
                Output<T>(Output<T> &source) = delete;

                //-----------------------------------------------------------//
                Output<T> &operator=(Output<T> &other) = delete;

                //-----------------------------------------------------------//
                Output<T>(Output<T> &&other) = default;

                //-----------------------------------------------------------//
                Output<T> &operator=(Output<T> &&other) = default;

                //-----------------------------------------------------------//
                ~Output<T>(){
                    dump();                                 // clear the buffer
                    output->flush();                        // clear the stream
                    if (outfile.is_open()){
                        outfile.close();                    // close file
                    }
                }

                //-Put-a-single-item-to-the-buffer---------------------------//
                void put(T &item){
                    (*current) = item;
                    current++;
                    if (current==end) dump();
                }

            private:

                //-Setup-buffer-and-output-----------------------------------//
                void init(){

                    // check how many elements fit into the buffer size
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    // allocate data buffer
                    databuffer = std::unique_ptr<T>(new T[n_elements]);

                    // set pointers for running the buffer
                    start = databuffer.get();
                    end = start+n_elements; 
                    current = start; 

                    // byte-view of the data buffer
                    bytebuffer = reinterpret_cast<const char *>( start );

                }

                // size_t total = 0;
                //-Write-the-buffer-content-and-reset-pointers---------------//
                void dump(){
                    output->write(bytebuffer, (current-start)*sizeof(T));
                    // total += (current-start)*sizeof(T);
                    current = start;
                }
        
                //-Handles---------------------------------------------------//
                std::ofstream outfile;
                std::ostream *output;

                //-Buffer-memory---------------------------------------------//
                std::unique_ptr<T> databuffer; 

                //-Pointers-for-buffer-management----------------------------//
                T *start;
                T *current;
                T *end;
            
                //-Byte-view-of-the-buffer-----------------------------------//
                char const *bytebuffer;
                
                //-Size-definitions------------------------------------------//
                size_t n_bytes;
                size_t n_elements;

        };


        //-------------------------------------------------------------------//
        template <typename T> class Input{
        
            public:

                //-----------------------------------------------------------//
                Input<T>()
                    : Input<T>(EMPTY_FILENAME, DEFAULT_SIZE)
                {}

                //-----------------------------------------------------------//
                Input<T>(size_t s)
                    : Input<T>(EMPTY_FILENAME, s)
                {}

                //-----------------------------------------------------------//
                Input<T>(std::string filename, size_t s=DEFAULT_SIZE) 
                    : n_bytes(s)
                {
                    if (filename != EMPTY_FILENAME){
                        // filename specified, open file and set to output
                        infile = std::ifstream(
                                filename, std::ifstream::binary);
                        input = &infile;
                    }
                    else{
                        // no filename, set to stdin
                        input = &std::cin;
                    }
                    init();
                }

                //-----------------------------------------------------------//
                Input<T>(Input<T> &source) = delete;
                
                //-----------------------------------------------------------//
                Input<T> &operator=(Input<T> &other) = delete;
                
                //-----------------------------------------------------------//
                Input<T>(Input<T> &&other) = default;
                
                //-----------------------------------------------------------//
                Input<T> &operator=(Input<T> &&other) = default;
                
                //-----------------------------------------------------------//
                ~Input<T>(){
                    if (infile.is_open()) infile.close(); // close file
                }

                //-Read-a-single-element-------------------------------------//
                bool get(T &target){
                    if (current >= end) fill();
                    target = *current;
                    current++;
                    return !eof || current < end; // not eof or data in buffer
                }

                //-----------------------------------------------------------//
                T peek() {
                    if (current >= end) fill();
                    return *current;
                }

                //-Make-inputs-sortable-by-next-value------------------------//
                bool operator< (const Input<T>& rhs) const {
                    return this->peek() < rhs.peek();
                }

            private:

                //-----------------------------------------------------------//
                void init(){

                    // check how many elements fit into the buffer size
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    // allocate data buffer
                    databuffer = std::unique_ptr<T>(new T[n_elements]);

                    // set pointers for running the buffer
                    start = databuffer.get();
                    end = start;
                    current = start;

                    // byte view of the buffer
                    bytebuffer = reinterpret_cast<char *>( start ) ;

                    // initial read
                    fill();

                }
    
                //-Fill-the-buffer-from-the-instream-------------------------//
                void fill(){
                    input->read(bytebuffer, n_bytes);
                    end = start + input->gcount()/sizeof(T);
                    current = start;
                    //eof = input->eof();
                    if (!input->gcount()) eof = true;
                }


                //-Async-IO--------------------------------------------------//

                //-Handles---------------------------------------------------//
                std::ifstream infile;
                std::istream *input;
                
                //-Buffer-memory---------------------------------------------//
                std::unique_ptr<T> databuffer; 

                //-Pointers-for-buffer-management----------------------------//
                T *start;
                T *current;
                T *end;
                
                //-Byte-view-of-the-bufferj----------------------------------//
                char *bytebuffer;

                //-Size-definitions------------------------------------------//
                size_t n_bytes;
                size_t n_elements;
                
                //-EOF-flag--------------------------------------------------//
                bool eof = false;

        };

    };

};

#endif
