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
        std::string const COMMANDLINE_KEY = "COMMANDLINE";
        std::string const GLOBALS_KEY = "GLOBALS";        
        std::string const ENVIRONMENT_KEY = "ENVIRONMENT";
        std::string const META_INFO_KEY = "METAINFO";        
        std::string const JSON_FILE_TOKEN = ".json"; 
        std::string const OPTION_TOKEN = "-";

        //-------------------------------------------------------------------//
        class JsonCLI{

            public:

                //-----------------------------------------------------------//
                JsonCLI(
                        int argc,
                        char* argv[],
                        std::vector<std::string> env_vars = {},
                        bool load_files = true,
                        bool flatten = false,
                        bool store_origin = false
                       ) :
                load_files(load_files),
                store_origin(store_origin)
                {

                    // exec name
                    parse_argv_vector(argc, argv);
                    args[META_INFO_KEY]["execname"] = argv_vec[0];

                    // env vars
                    for (auto &it : env_vars) {
                        args[ENVIRONMENT_KEY][it] = string_to_json(get_env_var(it));
                    }

                    // globals
                    auto it = argv_vec.begin()+1;
                    while(it != argv_vec.end() && !is_option(*it)){
                        globals_vec.push_back(*it++);;
                    }
                    args[GLOBALS_KEY] = vec_to_json(globals_vec);

                    std::string key = "";
                    if (it != argv_vec.end()) key = strip_cmd_arg(*it);
                    else {
                        // no named cmd args
                        args[COMMANDLINE_KEY] = json{};
                        return;
                    }
                    std::vector<std::string> parameters{};

                    // the rest are cmd arguments
                    while(it != argv_vec.end()){
                        cmd_args_vec.push_back(*it++);
                    }

                    // parse other options in their scope (flattening)
                    for (auto it = cmd_args_vec.begin(); it != cmd_args_vec.end(); ++it){
                        if (is_option(*it)){
                            if (args[COMMANDLINE_KEY].count(key)){
                                for (auto it : vec_to_json(parameters, flatten)) {
                                    args[COMMANDLINE_KEY][key].push_back(it);
                                }
                            }
                            else{
                                args[COMMANDLINE_KEY][key] = vec_to_json(parameters, flatten);
                            }
                            key = strip_cmd_arg(*it);
                            parameters.clear();
                        } 
                        else{
                            parameters.push_back(*it);
                        }
                    }

                    if (args[COMMANDLINE_KEY].count(key)){
                        for (auto it : vec_to_json(parameters, flatten)) {
                            args[COMMANDLINE_KEY][key].push_back(it);
                        }
                    }
                    else{
                        args[COMMANDLINE_KEY][key] = vec_to_json(parameters, flatten);
                    }

                }

                //-----------------------------------------------------------//
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
                bool is_option(std::string s){
                    return s.substr(0, 1) == OPTION_TOKEN;
                }

                //-----------------------------------------------------------//
                bool is_json_file_specifier(std::string s){
                    return (s.length() > 5) && 
                        (s.substr(s.length()-5, 5) == JSON_FILE_TOKEN);
                }

                //-----------------------------------------------------------//
                std::string strip_cmd_arg(std::string arg){
                    int count = 0;
                    while(count < arg.length() && arg.at(count) == '-') count++;
                    if (count >= arg.length()) return arg;
                    return arg.substr(count, arg.length()-count);
                }

                //-----------------------------------------------------------//
                json string_to_json(std::string s){
                    json entry{};
                    if (is_json_file_specifier(s) && load_files){
                        entry = file_to_json(strip_cmd_arg(s), store_origin);
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
                        bool flatten = false){
                    if (strings.size() == 0 && flatten){
                        return json(true);
                    }
                    if (strings.size() == 1 && flatten){
                        return string_to_json(strings[0]);
                    }
                    else{
                        json ary = json::array();
                        for (auto &it : strings){
                            ary.push_back(string_to_json(it));
                        }
                        return ary;
                    }
                }

                //-----------------------------------------------------------//
                json file_to_json(std::string const filename, bool origin){
                    std::ifstream infile{filename};
                    json js;
                    try{
                        infile >> js;
                    } catch (std::exception e){
                        std::cerr << "Error loading external file: ";
                        std::cerr << filename << std::endl;
                    }
                    infile.close();
                    if (origin){
                        json entry;
                        entry["_content"] = js;
                        entry["_origin"] = filename;
                        return entry;
                    }
                    else{
                        return js;
                    }
                }


                //-----------------------------------------------------------//
                std::string get_env_var(std::string env_var_name){
                    char * ENV_VAR_CSTRING = new char[256];
                    std::strcpy(ENV_VAR_CSTRING, env_var_name.c_str());
                    const char * path = new char[256];
                    path = std::getenv(ENV_VAR_CSTRING);
                    if (path != NULL){
                        return std::string(path);
                    }else{
                        delete[] path;
                    }
                    delete[] ENV_VAR_CSTRING;
                    return EMPTY_STRING;
                }
                
                //-----------------------------------------------------------//
                std::vector<std::string> argv_vec{};
                std::vector<std::string> env_vars{};
                std::vector<std::string> globals_vec{};
                std::vector<std::string> cmd_args_vec{};

                bool load_files = true;
                bool store_origin = false;

        };
    }
}

#endif
