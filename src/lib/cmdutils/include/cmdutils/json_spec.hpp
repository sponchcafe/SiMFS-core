#ifndef SIM_CMDUTILS_JSONSPEC_H
#define SIM_CMDUTILS_JSONSPEC_H

#include <string>
#include <json/json.hpp>

using json = nlohmann::json;

namespace jcli{


    class JsonSpec{

        public:

            //---------------------------------------------------------------//
            JsonSpec(JsonCli cli, std::string const scope)
                : scope(scope), cli(cli)
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
                T val = query_cli(name, shell_aliases, default_val, validator);
                values[name] = val;
                
                return val;

            }

            void enable_help(){
                if (cli.info()){
                    for (auto entry=defaults.begin(); entry!=defaults.end(); ++entry){
                        std::cout << entry.key() << "\t[";
                        for (auto alias : aliases[entry.key()]){
                            std::cout << alias << ", ";
                        }
                        std::cout << "] : \t"; 
                        std::cout << descriptions[entry.key()];
                        std::cout << std::endl;
                    }
                }
            }

            void enable_debug(){
                if (cli.debug()){
                    std::cout << cli.cli.dump(4) << std::endl;
                }
            }


            //---------------------------------------------------------------//
            void enable_config(){
                if (cli.config()){
                    json info_params = cli.cli[SHELL_KEY][FILE_TOKEN];
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
            }

            std::string const scope;

            json values;
            json aliases;
            json descriptions; 
            json defaults;

        private:

            JsonCli cli;

            //---------------------------------------------------------------//
            template <typename T> T assign_from_array(json array, T default_val,
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
            json collect_key(json object, std::string scope, std::string key, bool unwind=false){
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
            template <typename T> T query_cli(
                    std::string key,
                    std::vector<std::string> shell_aliases={},
                    T default_val = T{},
                    std::function<bool(T)> validator
                    = [](T val) -> bool {return true;}){

                T val = default_val;
                std::reverse(shell_aliases.begin(), shell_aliases.end());

                // search env
                json jvals_env;
                for (std::string envkey : cli.get_expanded_env()){
                    for (json object : cli.cli[jcli::ENV_KEY][envkey]){
                        jvals_env = collect_key(object, scope, key);
                        val = assign_from_array<T>(jvals_env, val, validator);
                    }
                }

                // search args
                json jvals_args;
                for (json object : cli.cli[jcli::SHELL_KEY][jcli::OPTION_TOKEN]){
                    jvals_args = collect_key(object, scope, key);
                    val = assign_from_array<T>(jvals_args, val, validator);
                }

                // search options
                json jvals_shell;
                // search aliases
                for (auto alias: shell_aliases){
                    jvals_shell = collect_key(cli.cli, jcli::SHELL_KEY, alias, true);
                    val = assign_from_array<T>(jvals_shell, val, validator);
                }
                // search name
                jvals_shell = collect_key(cli.cli, jcli::SHELL_KEY, key, true);
                val = assign_from_array<T>(jvals_shell, val, validator);

                return val;
            }
    };

}

#endif
