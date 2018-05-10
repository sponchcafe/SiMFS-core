#ifndef SIM_FILE_IO_H
#define SIM_FILE_IO_H

#include <iostream>
#include <fstream> 
#include <string>

#include "io/base_io.hpp"

namespace sim{
    using namespace comp;
    namespace file_io{

        constexpr unsigned DEFAULT_SIZE = 1024;

        //-------------------------------------------------------------------//
        template <typename T> class FileOutput : public Output<T>{

            public:

                //-----------------------------------------------------------//
                FileOutput<T>() : Output<T>() {};

                //-----------------------------------------------------------//
                FileOutput<T>(std::string filename) :
                    Output<T>{filename},
                    n_bytes(DEFAULT_SIZE) {
                        // filename specified, open file and set to output
                        outfile = std::unique_ptr<std::ofstream>(
                                new std::ofstream(filename, std::ofstream::binary)
                                );
                        output = outfile.get();
                        init();
                    }

                //-Copy-ctor:-DELETED----------------------------------------//
                FileOutput<T>(FileOutput<T> &source) = delete;

                //-Copy-assgin:-DELETED--------------------------------------//
                FileOutput<T> &operator=(FileOutput<T> &other) = delete;

                //-Move-ctor:-DEFAULT----------------------------------------//
                FileOutput<T>(FileOutput<T> &&other) = default;

                //-Move-assign:-DEFAULT--------------------------------------//
                FileOutput<T> &operator=(FileOutput<T> &&other) = default;

                //-----------------------------------------------------------//
                ~FileOutput<T>() {
                    dump();                                 // clear the buffer
                    output->flush();                        // clear the stream
                }

                //-Put-a-single-item-to-the-buffer---------------------------//
                void put(T &item) override {
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

                //-Write-the-buffer-content-and-reset-pointers---------------//
                void dump(){
                    output->write(bytebuffer, (current-start)*sizeof(T));
                    // total += (current-start)*sizeof(T);
                    current = start;
                }

                //-Handles---------------------------------------------------//
                std::unique_ptr<std::ofstream> outfile;
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
        template <typename T> class FileInput : public Input<T>{

            public:

                //-----------------------------------------------------------//
                FileInput<T>() : Input<T>() {};

                //-----------------------------------------------------------//
                FileInput<T>(std::string filename) :
                    Input<T>{filename},
                    n_bytes(DEFAULT_SIZE)
                    {
                        // filename specified, open file and set to output
                        infile = std::unique_ptr<std::ifstream>(
                                new std::ifstream(filename, std::ifstream::binary)
                                );
                        input = infile.get();
                        init();
                    }

                //-Copy-ctor:-DELETED----------------------------------------//
                FileInput<T>(FileInput<T> &source) = delete;

                //-Copy-assign:-DELETED--------------------------------------//
                FileInput<T> &operator=(FileInput<T> &other) = delete;

                //-Move-ctor:-DEFAULT----------------------------------------//
                FileInput<T>(FileInput<T> &&other) = default;

                //-Move-assign:-DEFAULT--------------------------------------//
                FileInput<T> &operator=(FileInput<T> &&other) = default; 

                //-Default-dtor----------------------------------------------//
                ~FileInput<T>() {}

                //-Read-a-single-element-------------------------------------//
                bool get(T &target) override {
                    if (buffer_empty) return false; // done
                    target = *current;
                    current++;
                    if (current >= end) fill();
                    return true;
                }

                //---------------------------------------------------------------//
                T peek() const override{
                    return *current;
                }

            private:

                //---------------------------------------------------------------//
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
                    buffer_empty = (current >= end);
                }

                //-Handles---------------------------------------------------//
                std::unique_ptr<std::ifstream> infile;
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
                bool buffer_empty = false;

        };

    }

}

#endif
