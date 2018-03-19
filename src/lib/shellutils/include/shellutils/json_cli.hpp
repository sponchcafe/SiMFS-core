#ifndef SIM_CMDUTILS_JSONCLI_H
#define SIM_CMDUTILS_JSONCLI_H

#include <iostream>
#include <fstream> 
#include <sstream> 
#include <string>
#include <unistd.h> // getpid

#include <json/json.hpp>

using json = nlohmann::json;

namespace jcli{

    std::string const EMPTY_STRING = "";        
    std::string const META_KEY = "meta";        
    std::string const ENV_KEY = "env";
    std::string const SHELL_KEY = "shell";
    std::string const FILE_TOKEN = "@"; 
    std::string const OPTION_TOKEN = "-";

    class JsonCli{

        public:

            //---------------------------------------------------------------//
            JsonCli(int argc, char* argv[], char** envp, 
                    std::vector<std::string> expand_env={}){
                cli[META_KEY]["execname"] = argv[0];
                cli[META_KEY]["pid"] = ::getpid();
                cli[SHELL_KEY] = parse_argv_vector(argc, argv);
                cli[ENV_KEY] = parse_envp_vector(envp);
                for (auto &it : expand_env) expand_env_var(it);
            }

            //---------------------------------------------------------------//
            bool option_present(std::string const option_name){
                return cli[SHELL_KEY].count(option_name) > 0;
            }

            //---------------------------------------------------------------//
            json expand_env_var(std::string key){
                expanded_env_vars.push_back(key);
                std::string value = EMPTY_STRING;
                try{
                    value = cli[ENV_KEY][key];
                } catch (std::exception e){
                    return json{};
                }
                std::vector<std::string> params = split_string(value);
                json jvalue = vec_to_json(params);
                cli[ENV_KEY][key] = jvalue;
                return jvalue;
            }

            //---------------------------------------------------------------//
            std::vector<std::string> get_expanded_env(){
                return expanded_env_vars;
            }

            //---------------------------------------------------------------//
            json cli{};

        private:

            //---------------------------------------------------------------//
            json parse_argv_vector(int argc, char * argv[]){
                std::vector<std::string> vec;
                for (int i=0; i<argc; i++){
                    vec.push_back(std::string(argv[i]));
                }

                json shell_input{};
                std::vector<std::string> params;
                std::string key = OPTION_TOKEN;

                for (auto it=vec.begin()+1; it!=vec.end(); ++it){
                    if (is_option(*it)){
                        append_assign_json(shell_input, key, params);
                        params.clear();
                        key = strip_cmd_arg(*it);
                    }
                    else{
                        params.push_back(*it);
                    }
                }
                append_assign_json(shell_input, key, params);

                return shell_input;
            }

            //---------------------------------------------------------------//
            void append_assign_json(
                    json &js_parent, std::string key, 
                    std::vector<std::string> &params){
                json js_child = vec_to_json(params);
                if (js_parent.count(key)){
                    for (auto &it : js_child){
                        js_parent[key].push_back(it);
                    }
                }
                else{
                    js_parent[key] = js_child;
                }
            }

            //---------------------------------------------------------------//
            json parse_envp_vector(char ** envp){
                std::vector<std::string> split;
                json env_input{};
                for (char **env = envp; *env != 0; env++){
                    std::string key_value_pair(*env);
                    split = split_string(key_value_pair, '=');
                    env_input[split[0]] = split[1];
                } 
                return env_input;
            }

            //---------------------------------------------------------------//
            bool has_prefix(std::string s, std::string prefix){
                return (s.length() >= prefix.length()) && 
                    (s.substr(0, prefix.length()) == prefix);
            }

            //---------------------------------------------------------------//
            bool is_option(std::string s){
                return has_prefix(s, OPTION_TOKEN);
            }

            //---------------------------------------------------------------//
            bool is_file_specifier(std::string s){
                return has_prefix(s, FILE_TOKEN);
            }

            //---------------------------------------------------------------//
            std::vector<std::string> split_string(std::string s, 
                    char delimiter=' '){
                std::vector<std::string> vec{};
                std::string item;
                std::stringstream ss(s);
                while(std::getline(ss, item, delimiter)){
                    if (item != EMPTY_STRING) vec.push_back(item);
                }
                return vec;
            }

            //---------------------------------------------------------------//
            std::string strip_cmd_arg(std::string arg){
                unsigned count = 0;
                while(count < arg.length() && arg.at(count) == '-') count++;
                if (count >= arg.length()) return "-";
                return arg.substr(count, arg.length()-count);
            }

            //---------------------------------------------------------------//
            std::string strip_file_specifier(std::string filespec){
                return filespec.substr(FILE_TOKEN.length(), 
                        filespec.length()-FILE_TOKEN.length());
            }

            //---------------------------------------------------------------//
            json string_to_json(std::string s){
                json entry{};
                if (is_file_specifier(s)){
                    s = load_file(strip_file_specifier(s));
                }
                try {
                    entry = json::parse(s);
                }
                catch (std::exception e) {
                    entry = s;
                }
                return entry;
            }

            //---------------------------------------------------------------//
            json vec_to_json(std::vector<std::string> &strings){
                json ary = json::array();
                for (auto &it : strings){
                    ary.push_back(string_to_json(it));
                }
                return ary;
            }

            //---------------------------------------------------------------//
            std::string load_file(std::string const filename){
                std::ifstream infile{filename};
                std::stringstream ss;
                ss << infile.rdbuf();
                if (!infile.good()){
                    return "LAOD_ERROR:"+filename;
                }
                infile.close();
                return ss.str();
            }


            std::vector<std::string> expanded_env_vars{};

    };

}

#endif
