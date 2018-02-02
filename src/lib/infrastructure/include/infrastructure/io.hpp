#ifndef SIM_INFRASTUCTURE_IO_H
#define SIM_INFRASTUCTURE_IO_H

#include "infrastructure/definitions.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>


namespace sim{
    namespace io{

        //-------------------------------------------------------------------//
        size_t const DEFAULT_SIZE = 1024;


        //-------------------------------------------------------------------//
        template <typename T> std::ostream & write_binary(
                std::ostream& os, T const & t){
            char const * out_buffer = reinterpret_cast<char const *>(&t);
            return os.write(out_buffer, sizeof(T));
        }


        //-------------------------------------------------------------------//
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
                Output<T>(std::string filename="", size_t s=DEFAULT_SIZE) : 
                    n_bytes(s){
                    if (filename != ""){
                        outfile = new std::ofstream(filename, 
                                std::ofstream::binary);
                        output = outfile;
                    }else{
                        output = &std::cout;
                    }
                    init();
                }

                //-----------------------------------------------------------//
                Output<T>(size_t s) : n_bytes(s){
                    output = &std::cout;
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
                    output->flush();
                    if (current>start){
                        dump();
                    }
                    output->flush(); 
                }

                //-----------------------------------------------------------//
                void put(T &item){
                    (*current) = item;
                    current++;
                    if (current==end){
                        dump();
                        current = start;
                    }
                }

            private:

                //-----------------------------------------------------------//
                void init(){
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    databuffer = std::unique_ptr<T>(new T[n_elements]);
                    start = databuffer.get();
                    end = start+n_elements;
                    current = start;

                    bytebuffer = reinterpret_cast<const char *>( start );
                }

                //-----------------------------------------------------------//
                void dump(){
                    output->write(bytebuffer, (current-start)*sizeof(T));
                }
        
                //-----------------------------------------------------------//
                std::ofstream *outfile;
                std::ostream *output;

                //-----------------------------------------------------------//
                std::unique_ptr<T> databuffer; 

                T *start;
                T *current;
                T *end;
            
                const char *bytebuffer;
                
                size_t n_bytes;
                size_t n_elements;

        };

        template <typename T> class Input{
        
            public:

                //-----------------------------------------------------------//
                Input<T>(std::istream &in=std::cin, size_t s=DEFAULT_SIZE) 
                    : input(&in), n_bytes(s){
                    init();
                }

                //-----------------------------------------------------------//
                Input<T>(std::string filename="", size_t s=DEFAULT_SIZE) 
                    : n_bytes(s){
                    if (filename != ""){
                        infile = new std::ifstream(
                                filename, std::ifstream::binary);
                        input = infile;
                    }else{
                        input = &std::cin;
                    }
                    init();
                }

                //-----------------------------------------------------------//
                Input<T>(size_t s) : n_bytes(s){
                    input = &std::cin;
                    init();
                }

                //-----------------------------------------------------------//
                Input<T>(Input<T> &source) = delete;
                Input<T> &operator=(Input<T> &other) = delete;
                Input<T>(Input<T> &&other) = default;
                Input<T> &operator=(Input<T> &&other) = default;
                ~Input<T>(){}

                //-----------------------------------------------------------//
                bool get(T &target){
                    if (current >= end){
                        fill();
                    }
                    target = *current;
                    current++;
                    return !eof || current < end;
                }

                //-----------------------------------------------------------//
                T peek() const {
                    return *current;
                }

                //-----------------------------------------------------------//
                bool operator< (const Input<T>& rhs) const {
                    return this->peek() < rhs.peek();
                }

            private:

                //-----------------------------------------------------------//
                void init(){
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    databuffer = std::unique_ptr<T>(new T[n_elements]);
                    start = databuffer.get();
                    end = start;
                    current = start;

                    bytebuffer = reinterpret_cast<char *>( start ) ;
                    fill();
                }
    
                //-----------------------------------------------------------//
                void fill(){
                    input->read(bytebuffer, n_bytes);
                    end = start + input->gcount()/sizeof(T);
                    current = start;
                    if (!input->gcount()){
                        eof = true;
                    }
                }

                std::ifstream *infile;
                std::istream *input;
                
                //-----------------------------------------------------------//
                std::unique_ptr<T> databuffer; 

                T *start;
                T *current;
                T *end;
                
                char *bytebuffer;
                size_t n_bytes;
                size_t n_elements;
                
                bool eof = false;

        };

    };

};
#endif
