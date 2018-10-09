#include <cstdlib>
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
    double get_env(std::string name, double def){
        char const *value = std::getenv(name.c_str());
        if (value == nullptr) return def;
        std::cerr << name << " set to " << value << '\n';
        return std::stod(value);
    }

}
