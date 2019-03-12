#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include "json/json.hpp"
#include "platform.hpp"

#ifdef WINDOWS
#include <io.h>
#else
#include <unistd.h>
#endif


using json = nlohmann::json;

namespace sim{
    namespace cli{

        std::string const HELP_TOKEN = "help";
        std::string const CONF_TOKEN = "list";

        //-------------------------------------------------------------------//
        std::vector<std::string> parse_argv_vector(int argc, char * argv[]){
            std::vector<std::string> vec;
            for (int i=0; i<argc; i++){
                vec.push_back(std::string(argv[i]));
            }
            return vec;
        }

        //-------------------------------------------------------------------//
        bool search_token(std::vector<std::string> &argvec, std::string token){
            for (auto &it: argvec){
                if (it == token) {
                    return true;
                }
            }
            return false;
        }

        //-Check-for-list-option---------------------------------------------//
        bool check_list(std::vector<std::string> &argv){
            return search_token(argv, "list");
        }

        //-Get-parameters----------------------------------------------------//
        json get_parameters(){

            std::stringstream ss; 
            std::string s;
            json j{};

            if (isatty(fileno(stdin))){
                std::cerr << "Using defaults only.\n";
                return json::object();
            }

            if (std::cin.peek() == EOF){
                std::cerr << "Empty parameter input.\n";
                return json::object();
            }

            while (std::getline(std::cin, s)){
                ss << s;
            }

            try{
                ss >> j;
            }catch(nlohmann::detail::parse_error &e){
                std::cerr << "Error parsing json parameters.\n";
                return json::object();
            }
            return j;
        }

        //-Log---------------------------------------------------------------//
        void log_parameters(json j){
            std::cout << j.dump(4) << std::endl;
        }

    }
}

