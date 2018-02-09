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
        std::string const ENVIRONMENT_KEY = "ENVIRONMENT";
        std::string const EXECNAME_KEY = "EXECNAME";        
        std::string const JSON_FILE_TOKEN = ".json"; std::string const SHORT_OPTION_TOKEN = "-";
        std::string const LONG_OPTION_TOKEN = "--";

        //-------------------------------------------------------------------//
        class JsonCLI{

            public:

                //-----------------------------------------------------------//
                JsonCLI(
                        int argc,
                        char* argv[],
                        bool load_files = true,
                        std::vector<std::string> env_vars = {}
                       ) : env_vars(env_vars), load_files(load_files)
                {

                    parse_argv_vector(argc, argv);
                    args[EXECNAME_KEY] = argv_vec[0];

                    auto pos = argv_vec.begin()+1;

                    // inject environment and globals
                    pos = argv_vec.insert(pos, "--"+GLOBALS_KEY);
                    pos = argv_vec.insert(pos, "--"+ENVIRONMENT_KEY);
                    ++pos;
                    for (auto &it : env_vars) argv_vec.insert(pos, it);

                    for (auto &it : argv_vec) std::cout << it << '\n';
                    // parse the modified argv vector
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
                json string_to_json(std::string s){
                    json entry{};
                    if (is_json_file_specifier(s) && load_files){
                        entry = file_to_json(strip_cmd_arg(s));
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
                json file_to_json(std::string const filename){
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
                void argv_to_json(){

                    std::string key = GLOBALS_KEY;
                    std::vector<std::string> parameters{};

                    // parse other options in their scope (flattening)
                    for (auto it = argv_vec.begin(); it != argv_vec.end(); ++it){
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
                std::vector<std::string> env_vars;
                bool load_files = true;

        };
    }
}

#endif
