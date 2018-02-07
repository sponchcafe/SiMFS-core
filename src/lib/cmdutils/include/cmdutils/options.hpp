#ifndef SIM_CMDUTILS_OPTIONS_H
#define SIM_CMDUTILS_OPTIONS_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include <getopt/getopt_pp.hpp>
#include <json/json.hpp>

using json = nlohmann::json;

namespace sim{
    namespace opt{

        std::string const empty_string = "";        
        const std::string opt_helpmessage = R"(
    Reserved:
    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'params.json'.
    -c --config : Generate an example .json paramter string and print it to the standard output.
    -h --help : Show this help message.
)";

        //-------------------------------------------------------------------//
        class ParameterHandler{

            public:

                //-----------------------------------------------------------//
                ParameterHandler(){}

                //-----------------------------------------------------------//
                ParameterHandler(
                        std::string const prefix,
                        json params,
                        int argc,
                        char* argv[]) :
                    prefix(prefix){

                    input_params = params[prefix];

                    ops_noenv.reset(new GetOpt::GetOpt_pp(argc, argv));
                    ops_env.reset(new GetOpt::GetOpt_pp(argc, argv, 
                                GetOpt::Include_Environment));
            }


                //-----------------------------------------------------------//
                template <typename T> void get_parameter(
                        std::string const name,
                        char const shorthand,
                        T default_value,
                        std::string const description){
                    
                    // Stage 1: Default value
                    params[name] = default_value;

                    // Stage 2: Environment variables
                    params[name] = ops_env->getopt<T>(
                            shorthand, name, params[name]);

                    // Stage 3: JSON input parameters
                    if (input_params.count(name)) {
                        params[name] = input_params[name];
                    }
                    
                    // Stage 4: Command line arguments
                    params[name] = ops_noenv->getopt<T>(
                            shorthand, name, params[name]);

                    params_info[name]["desc"] = description;

                }

                //-----------------------------------------------------------//
                void enableConfig() {
                    *ops_noenv >> GetOpt::OptionPresent('C', "config", debug);
                    if (debug){
                        std::cout << params.dump(4);
                        std::cout << std::endl;
                    }   
                }

                //-----------------------------------------------------------//
                void enableHelp(std::string helpmessage);

                bool is_good(){
                    return good;
                }
                bool in_debug_mode(){
                    return debug;
                }

                json params {};

            private:

                bool debug = false;
                bool good = true;

                std::string prefix;
                json input_params {};
                json params_info {};
                std::unique_ptr<GetOpt::GetOpt_pp> ops_noenv;
                std::unique_ptr<GetOpt::GetOpt_pp> ops_env;

        };
    };
}

#endif
