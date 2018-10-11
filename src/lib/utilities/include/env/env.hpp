#pragma once
#include <string>

namespace sim{

    //-------------------------------------------------------------------//
    std::string get_env(std::string name, std::string def="");

    //-------------------------------------------------------------------//
    int get_env(std::string name, int def=0);
    unsigned long int get_env_bytes(std::string name, unsigned long int def=0);
    unsigned long int get_env_nanos(std::string name, unsigned long int def=0);

    //-------------------------------------------------------------------//
    double get_env(std::string name, double def=0);

}
