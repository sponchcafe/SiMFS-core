#ifndef SIM_PARAMS_H
#define SIM_PARAMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include <getopt/getopt_pp.hpp>
#include <json/json.hpp>

using json = nlohmann::json;

namespace sim{

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

                void enableConfig();
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
