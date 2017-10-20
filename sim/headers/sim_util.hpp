#ifndef SIM_UTIL_H
#define SIM_UTIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "getopt_pp.hpp"
#include "json.hpp"

#define CONST_C 299792458.0
#define CONST_H 6.62607004e-34
#define CONST_NA 6.022140857e+23
#define CONST_PI 3.141592653589793
#define DEFAULT_SIZE 1024

using json = nlohmann::json;

namespace sim{

    namespace log{
    
        void info(std::string msg);
        void warn(std::string msg);
        void critical(std::string msg);

    };

    namespace io{

        typedef struct{
            int16_t x;
            int16_t y;
            int16_t z;
        } coordinate;

        typedef double timetag;
        typedef double flux;
        typedef double efficiency;
        typedef uint32_t photon_count;

        template <typename T> std::ostream& write_binary(std::ostream& os, const T& t){
            const char *out_buffer = reinterpret_cast<const char *>(&t);
            return os.write(out_buffer, sizeof(T));
        }

        template <typename T> std::istream& read_binary(std::istream &is, T& t){
            char *in_buffer = reinterpret_cast<char *>(&t);
            is.read(in_buffer, sizeof(t));
                if (!is){
                    is.setstate(std::ios_base::failbit);
                return is;
            }
            return is;
        }
        
        /* fast file io */
        template <typename T> class Output{

            public:
            
                Output<T>(std::string filename="", size_t s=DEFAULT_SIZE) : n_bytes(s){
                    if (filename != ""){
                        outfile = std::ofstream(filename, std::ofstream::binary);
                        output = &outfile;
                    }else{
                        output = &std::cout;
                    }
                    init();
                }

                Output<T>(size_t s) : n_bytes(s){
                    output = &std::cout;
                    init();
                }

                ~Output<T>(){
                    if (current>start){
                        dump();
                    }
                    delete(start);
                }

                void put(T &item){
                    (*current) = item;
                    current++;
                    if (current==end){
                        dump();
                        current = start;
                    }
                }

            private:

                void init(){
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    start = new T[n_elements];
                    end = start+n_elements;
                    current = start;

                    bytebuffer = reinterpret_cast<const char *>( start );
                }
                
                void dump(){
                    output->write(bytebuffer, (current-start)*sizeof(T));
                }
        
                std::ofstream outfile;
                std::ostream *output;

                T *start;
                T *current;
                T *end;
            
                const char *bytebuffer;
                
                size_t n_bytes;
                size_t n_elements;

        };

        template <typename T> class Input{
        
            public:

                Input<T>(std::istream &in=std::cin, size_t s=DEFAULT_SIZE) : input(&in), n_bytes(s){
                    init();
                }

                Input<T>(std::string filename="", size_t s=DEFAULT_SIZE) : n_bytes(s){
                    if (filename != ""){
                        infile = std::ifstream(filename, std::ifstream::binary);
                        input = &infile;
                    }else{
                        input = &std::cin;
                    }
                    init();
                }

                Input<T>(size_t s) : n_bytes(s){
                    input = &std::cin;
                    init();
                }

                ~Input<T>(){
                    delete(start);
                }

                bool get(T &target){
                    if (current >= end){
                        fill();
                    }
                    target = *current;
                    current++;
                    return !eof || current < end;
                }

            private:

                void init(){
                    n_elements = (n_bytes/sizeof(T));
                    n_bytes = n_elements * sizeof(T);
                    
                    start = new T[n_elements];
                    end = start;
                    current = start;
                    
                    bytebuffer = reinterpret_cast<char *>( start ) ;
                    fill();
                }
                
                void fill(){
                    input->read(bytebuffer, n_bytes);
                    end = start + input->gcount()/sizeof(T);
                    current = start;
                    if (!input->gcount()){
                        eof = true;
                    }
                }

                std::ifstream infile;
                std::istream *input;

                T *start;
                T *current;
                T *end;
                
                char *bytebuffer;
                size_t n_bytes;
                size_t n_elements;
                
                bool eof = false;

        };

    };

    namespace opt{
        const std::string empty = "";        
        const std::string opt_helpmessage = R"(
    Reserved:
    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'params.json'.
    -c --config : Generate an example .json paramter string and print it to the standard output.
    -h --help : Show this help message.
    
)";

        class Parameters{

            public:

                Parameters(int argc, char* argv[], const std::string rootname);

                GetOpt::GetOpt_pp *ops;
                json params;

                void enableConfig(bool terminate=true);
                void enableHelp(std::string helpmessage);

                template <typename T> T getOption(const char shortopt, const std::string longopt, T default_value){
                    try{
                        default_value = params.at(longopt);
                    } catch (std::exception &e){}
                    params[longopt] = ops->getopt<T>(shortopt, longopt, default_value);
                    return params[longopt];
                };

                private:
                    std::string filename;
                    std::string rootname;

        };
    };
}

#endif