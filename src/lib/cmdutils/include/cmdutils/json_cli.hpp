#ifndef SIM_CMDUTILS_JSONCLI_H
#define SIM_CMDUTILS_JSONCLI_H

#include <iostream>
#include <fstream> 
#include <string>
#include <stdlib.h>

#include <json/json.hpp>

using json = nlohmann::json;

namespace sim{
    namespace opt{

        std::string const EMPTY_STRING = "";        
        std::string const GLOBALS_KEY = "GLOBALS";        
        std::string const EXECNAME_KEY = "EXECNAME";        
        std::string const JSON_FILE_TOKEN = ".json";
        std::string const SHORT_OPTION_TOKEN = "-";
        std::string const LONG_OPTION_TOKEN = "--";
        std::string const ENV_VAR = "SIM_PARAM_FILE";

        //-------------------------------------------------------------------//
        class JsonCLI{

            public:

                //-----------------------------------------------------------//
                JsonCLI(
                        int argc,
                        char* argv[],
                        bool propagate_globals = true,
                        bool load_files = true,
                        bool include_env = true
                       ) : 
                propagate_globals(propagate_globals),
                load_files(load_files),
                include_env(include_env)
            {
                parse_argv_vector(argc, argv);
                argv_to_json();
                }

                json args{};

            private:

                //-----------------------------------------------------------//
                void parse_argv_vector(int argc, char * argv[]){
                    std::vector<std::string> vec;
                    for (int i=0; i<argc; i++){
                        vec.push_back(std::string(argv[i]));
                    }
                    argv_vec = vec;
                }

                //-----------------------------------------------------------//
                bool is_short_option(std::string s){
                    return (s.length() == 2) && (s.substr(0, 1) == SHORT_OPTION_TOKEN);
                }

                //-----------------------------------------------------------//
                bool is_long_option(std::string s){
                    return (s.length() > 2) && (s.substr(0, 2) == LONG_OPTION_TOKEN);
                }

                //-----------------------------------------------------------//
                bool is_option(std::string s){
                    return is_short_option(s) || is_long_option(s);
                }

                //-----------------------------------------------------------//
                bool is_json_file_specifier(std::string s){
                    return (s.length() > 5) && 
                        (s.substr(s.length()-5, 5) == JSON_FILE_TOKEN);
                }

                //-----------------------------------------------------------//
                std::string strip_cmd_arg(std::string arg){
                    if (is_long_option(arg)) {
                        return arg.substr(2, arg.size()-2);
                    }
                    if (is_short_option(arg)) {
                        return arg.substr(1, arg.size()-1);
                    }
                    return arg;
                }

                //-----------------------------------------------------------//
                json convert_to_json(std::string s){
                    json entry{};
                    if (is_json_file_specifier(s) && load_files){
                        entry = load_json(strip_cmd_arg(s));
                        if (entry == json{}) entry = s;
                        return entry;
                    }
                    try {
                        entry = json::parse(s);
                    }
                    catch (std::exception e) {
                        entry = s;
                    }
                    return entry;
                }

                //-----------------------------------------------------------//
                json vec_to_json(std::vector<std::string> &strings, 
                        bool flatten = true){
                    if (strings.size() == 0 && flatten){
                        return json(true);
                    }
                    if (strings.size() == 1 && flatten){
                        return convert_to_json(strings[0]);
                    }
                    else{
                        json ary = json::array();
                        for (auto &it : strings){
                            ary.push_back(convert_to_json(it));
                        }
                        return ary;
                    }
                }

                //-----------------------------------------------------------//
                json load_json(std::string const filename){
                    std::ifstream infile{filename};
                    json js;
                    try{
                        infile >> js;
                    } catch (std::exception e){
                        std::cerr << "Error loading external file: ";
                        std::cerr << filename << std::endl;
                    }
                    infile.close();
                    return js;
                }


                //-----------------------------------------------------------//
                void propagate(){
                    json globals = args[GLOBALS_KEY];
                    for (json::iterator jit = globals.begin(); 
                            jit != globals.end(); ++jit){
                        if (jit->is_object()){
                            json propagated = *jit;
                            for (json::iterator prop_it = propagated.begin(); 
                                    prop_it != propagated.end(); ++prop_it){
                                args[prop_it.key()] = prop_it.value();
                            }
                        }
                    }
                }

                //-----------------------------------------------------------//
                std::string get_env_var(){
                    char * ENV_VAR_CSTRING = new char[256];
                    std::strcpy(ENV_VAR_CSTRING, ENV_VAR.c_str());
                    const char * path = new char[256];
                    path = std::getenv(ENV_VAR_CSTRING);
                    if (path != NULL){
                        std::cerr << "getting " << path << std::endl;
                        return std::string(path);
                    }else{
                        delete[] path;
                    }
                    delete[] ENV_VAR_CSTRING;
                    return EMPTY_STRING;
                }
                
                //-----------------------------------------------------------//
                void argv_to_json(){

                    args = {};

                    // argv[0] == program name
                    auto it = argv_vec.begin();
                    args[EXECNAME_KEY] = *it; ++it;

                    // get globals (unnamed parameters)
                    args[GLOBALS_KEY] = json::array();

                    std::string key = GLOBALS_KEY;
                    std::vector<std::string> parameters{};
                    
                    // getenv
                    if (include_env){
                        std::string env = EMPTY_STRING;
                        env = get_env_var();
                        if (env != EMPTY_STRING){
                            parameters.push_back(env);
                        }
                    }

                    // Collect all globals
                    while(it != argv_vec.end() && !is_option(*it)){
                        parameters.push_back(*it);
                        ++it;
                    }

                    // build globals json array (no flattening)
                    args[GLOBALS_KEY] = vec_to_json(parameters, false);

                    // Propagate objects from globals to toplevel scope
                    if (propagate_globals) propagate();

                    // return if only globals
                    if (it == argv_vec.end()) return;

                    // parse other options in their scope (flattening)
                    for (; it != argv_vec.end(); ++it){
                        if (is_option(*it)){
                            args[key] = vec_to_json(parameters);
                            key = strip_cmd_arg(*it);
                            parameters.clear();
                        } 
                        else{
                            parameters.push_back(*it);
                        }
                    }
                    args[key] = vec_to_json(parameters);

                }

                //-----------------------------------------------------------//
                std::vector<std::string> argv_vec{};
                bool propagate_globals = true;
                bool load_files = true;
                bool include_env = true;

        };
    }
}

#endif
