#include <cstdlib>
#include <cctype> // isdigit
#include "env/env.hpp"
#include <iostream>

namespace sim{

    //-------------------------------------------------------------------//
    std::string get_env(std::string name, std::string def){
        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::cerr << name << " set to " << value << '\n';
        return std::string(value);
    }

    //-------------------------------------------------------------------//
    int get_env(std::string name, int def){
        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::cerr << name << " set to " << value << '\n';
        return std::stoi(value);
    }

    //-------------------------------------------------------------------//
    unsigned long int get_env_bytes(std::string name, unsigned long int def){

        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::string s = std::string(value);
        unsigned int num = std::stoi(value);
        unsigned long int factor = 1;

        if (!std::isdigit(s.back())) {
            switch (s.back()){
                case 'k' : factor = 1<<10; break;
                case 'M' : factor = 1<<20; break;
                case 'G' : factor = 1<<30; break;
                default  : factor = 1; break;
            }
        }

        std::cerr << name << " set to " << value << " [" << num*factor << " bytes]" << '\n';
        return num*factor;
    }

    //-------------------------------------------------------------------//
    unsigned long int get_env_nanos(std::string name, unsigned long int def){

        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::string s = std::string(value);
        unsigned int num = std::stoi(value);
        unsigned long int factor = 1;

        if (!std::isdigit(s.back())) {
            switch (s.back()){
                case 'n' : factor = 1;          break;
                case 'u' : factor = 1000;       break;
                case 'm' : factor = 1000000;    break;
                default  : factor = 1000000000; break;
            }
        }

        std::cerr << name << " set to " << value << " [" << num*factor << " nanoseconds]" << '\n';
        return num*factor;
    }

    //-------------------------------------------------------------------//
    double get_env(std::string name, double def){
        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::cerr << name << " set to " << value << '\n';
        return std::stod(value);
    }

}
