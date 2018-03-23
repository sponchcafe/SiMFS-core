#ifndef SIM_CMDUTILS_JSONSPEC_H
#define SIM_CMDUTILS_JSONSPEC_H

#include <string>
#include <sstream>
#include <json/json.hpp>

using json = nlohmann::json;

namespace jcli{

    class JsonSpec{

        public:

            //---------------------------------------------------------------//
            JsonSpec(JsonCli cli)
                : cli(cli)
            {
                values = json{};
            }

            //---------------------------------------------------------------//
            template<typename T> T get_option(
                    std::string const name,
                    std::vector<std::string> shell_aliases = {},
                    T default_val = T{},
                    std::string description = "",
                    std::function<bool(T)> validator 
                    = [](T val) -> bool {return true;}
                    ){

                descriptions[name] = description;
                aliases[name] = shell_aliases;
                defaults[name] = default_val;
                json collected_vals = query_cli(name, shell_aliases);

                T val = assign_from_array(collected_vals, default_val, validator);
                values[name] = val;
                
                return val;

            }

            //---------------------------------------------------------------//
            template<typename T> std::vector<T> get_option_list(
                    std::string const name,
                    std::vector<std::string> shell_aliases = {},
                    std::vector<T> default_val = std::vector<T>{},
                    std::string description = "",
                    std::function<bool(T)> validator 
                    = [](T val) -> bool {return true;}
                    ){
                descriptions[name] = description;
                aliases[name] = aliases;
                json collected_vals = query_cli(name, shell_aliases);
                values[name] = collected_vals;
                std::vector<T> list = default_val;
                if (collected_vals.is_null()) return list;
                for (json::iterator it = collected_vals.begin(); it != collected_vals.end(); ++it){
                    list.push_back(*it);
                }
                return list;
            }

            //---------------------------------------------------------------//
            void enable_scoping(){

                if(cli.option_present("scope")){
                    json scope_params = cli.cli[SHELL_KEY]["scope"];
                    for (std::string it: scope_params){
                        scope = json::json_pointer{it};
                        iscope = json::json_pointer{it};
                        oscope = json::json_pointer{it};
                    }
                }

                if(cli.option_present("iscope")){
                    json scope_params = cli.cli[SHELL_KEY]["iscope"];
                    for (std::string it: scope_params){
                        iscope = json::json_pointer{it};
                    }
                }

                if(cli.option_present("oscope")){
                    json scope_params = cli.cli[SHELL_KEY]["oscope"];
                    for (std::string it: scope_params){
                        oscope = json::json_pointer{it};
                    }
                }


            }

            //---------------------------------------------------------------//
            void enable_help(){
                if (cli.option_present("help")){
                    for (auto entry=defaults.begin(); entry!=defaults.end(); ++entry){
                        std::cout << entry.key() << "\t[";
                        for (auto alias : aliases[entry.key()]){
                            std::cout << alias << ", ";
                        }
                        std::cout << "] : \t"; 
                        std::cout << descriptions[entry.key()];
                        std::cout << std::endl;
                    }
                    exit(1);
                }
            }

            //---------------------------------------------------------------//
            void enable_debug(){
                if (cli.option_present("debug")){
                    std::cout << cli.cli.dump(4) << std::endl;
                    exit(1);
                }
            }

            //---------------------------------------------------------------//
            void enable_log(){
                if (cli.option_present("log")) dump_parameters("log");
            }

            //---------------------------------------------------------------//
            void enable_config(){
                if (cli.option_present("config")) {
                    dump_parameters("config");
                    exit(1);
                }
            }

            json::json_pointer scope{EMPTY_STRING};
            json::json_pointer iscope{EMPTY_STRING};
            json::json_pointer oscope{EMPTY_STRING};

            json values;
            json aliases;
            json descriptions; 
            json defaults;

        private:

            JsonCli cli;

            //---------------------------------------------------------------//
            void dump_parameters(std::string key){
                
                json params = cli.cli[SHELL_KEY][key];
               
                // No params (filenames) specified -> dump to stdout
                if (params.size() == 0){
                    json output;
                    output[oscope] = values;
                    std::cout << output.dump(4) << std::endl;
                }

                // For all params (filenames) specified -> dump to file
                for (auto it: params){
                    
                    // read file
                    std::string filename = it;
                    std::ifstream infile;
                    infile.open(it);
                    json jfile_before;

                    try{
                        infile >> jfile_before;
                    }
                    catch (std::exception e){}

                    infile.close();

                    // update
                    std::ofstream outfile;
                    outfile.open(filename);
                    jfile_before[oscope] = values;
                    outfile << jfile_before.dump(4) << std::endl;
                    outfile.close();
                }
            }

            //---------------------------------------------------------------//
            void prepend_to_scope(std::string s){
                if (s != EMPTY_STRING){
                    std::string prepend = "/"+s;
                    std::string scope_str = scope.to_string();
                    prepend += scope_str;
                    scope = json::json_pointer(prepend);
                }
            }


            //---------------------------------------------------------------//
            template <typename T> T assign_from_array(
                    json array, T default_val, 
                    std::function<bool(T)> validator){
                
                T val = default_val;
                T _val = default_val;

                for (auto it: array){ 
                    try {
                        _val = it; // type checking happens here
                    }
                    catch(std::exception){
                        std::cerr << "Value not accepted: " << it;
                        std::cerr << " -- Last valid value: " << _val << std::endl;
                    }
                    if (validator(_val)) {
                        val = _val;
                    }
                    else{
                        std::cerr << "Validation failed for value: " << _val;
                        std::cerr << " -- Last valid value: " << _val << std::endl;
                    }
                }

                return val;
            }


            //-Get-all-values-to-a-given-key---------------------------------//
            json collect_key(json object, json::json_pointer scope, std::string key, 
                    bool unwind=false){
                if (
                        object.is_null() ||
                        !object.is_object() ||
                        object[scope].is_null()
                        ) 
                {
                    return json::array(); // no object or no scope
                }

                json collected = json::array(); // collection array
                json val = object[scope][key]; // current value

                if (val.is_null()) {
                    return collected; // key not present or no parameters
                }

                if (unwind && val.is_array()){
                    for (auto item : val) {
                        collected.push_back(item);
                    }
                }
                else {
                    collected.push_back(val);
                }

                return collected;
            }

            //---------------------------------------------------------------//
            json query_cli(
                    std::string key,
                    std::vector<std::string> shell_aliases={}
                    ){

                json jvals;

                // search env
                json jvals_env;
                for (std::string envkey : cli.get_expanded_env()){
                    for (json object : cli.cli[jcli::ENV_KEY][envkey]){
                        jvals_env = collect_key(object, iscope, key);
                        for (auto it: jvals_env) jvals.push_back(it);
                    }
                }

                // search args
                json jvals_args;
                for (json object : 
                        cli.cli[jcli::SHELL_KEY][jcli::OPTION_TOKEN]
                        ){
                    jvals_args = collect_key(object, iscope, key);
                    for (auto it: jvals_args) jvals.push_back(it);
                }

                // search options
                json jvals_shell;

                // search aliases
                for (auto alias: shell_aliases){
                    jvals_shell = collect_key(
                            cli.cli, json::json_pointer{"/"+jcli::SHELL_KEY}, alias, true
                            );
                    for (auto it: jvals_shell) jvals.push_back(it);
                }

                // search name
                jvals_shell = collect_key(
                        cli.cli, json::json_pointer{"/"+jcli::SHELL_KEY}, key, true
                        );
                for (auto it: jvals_shell) jvals.push_back(it);

                return jvals;
            }
    };

}

#endif
