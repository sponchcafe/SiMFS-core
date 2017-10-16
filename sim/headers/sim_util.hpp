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
        typedef int16_t photon_count;

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
        
    };

    namespace opt{

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