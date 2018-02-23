#ifndef SIM_CMDUTILS_JSONSPEC_H
#define SIM_CMDUTILS_JSONSPEC_H

#include <string>
#include <json/json.hpp>

using json = nlohmann::json;

namespace jcli{

    class JsonSpec{

        public:

            //---------------------------------------------------------------//
            JsonSpec(JsonCli cli, std::string const scope_str = EMPTY_STRING)
                : cli(cli)
            {
                prepend_to_scope(scope_str);
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

            void enable_scoping(){
                if(cli.option_present("scope")){
                    json scope_params = cli.cli[SHELL_KEY]["scope"];
                    for (auto it: scope_params){
                        prepend_to_scope(it);
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

            json::json_pointer scope;

            json values;
            json aliases;
            json descriptions; 
            json defaults;

        private:

            JsonCli cli;

            //---------------------------------------------------------------//
            void dump_parameters(std::string key){
                json info_params = cli.cli[SHELL_KEY][key];
                if (info_params.size() == 0){
                    json output;
                    output[scope] = values;
                    std::cout << output.dump(4) << std::endl;
                }
                for (auto it: info_params){
                    std::string filename = it;
                    std::ifstream infile;
                    infile.open(it);
                    json jfile_before;
                    try{
                        infile >> jfile_before;
                    }
                    catch (std::exception e){}
                    infile.close();
                    std::ofstream outfile;
                    outfile.open(filename);
                    jfile_before[scope] = values;
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
                        _val = it;
                    }
                    catch(std::exception){};
                    if (validator(_val)) val = _val;
                }

                return val;
            }


            //---------------------------------------------------------------//
            json collect_key(json object, json::json_pointer scope, std::string key, 
                    bool unwind=false){
                if (
                        object.is_null() || 
                        !object.is_object() || 
                        object[scope].is_null()
                        ) 
                {
                    return json::array();
                }

                json collected = json::array();
                json val = object[scope][key];

                if (val.is_null()) {
                    return collected;
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

                //std::reverse(shell_aliases.begin(), shell_aliases.end());

                json jvals;

                // search env
                json jvals_env;
                for (std::string envkey : cli.get_expanded_env()){
                    for (json object : cli.cli[jcli::ENV_KEY][envkey]){
                        jvals_env = collect_key(object, scope, key);
                        for (auto it: jvals_env) jvals.push_back(it);
                    }
                }

                // search args
                json jvals_args;
                for (json object : 
                        cli.cli[jcli::SHELL_KEY][jcli::OPTION_TOKEN]
                        ){
                    jvals_args = collect_key(object, scope, key);
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
