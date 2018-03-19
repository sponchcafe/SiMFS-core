#include "shellutils/shellutils.hpp"
#include <iostream>

int main(int argc, char * argv[], char **envp){

    jcli::JsonCli cli{
        argc, argv, envp, 
        {
            "SIM_PARAM_FILE", 
            "SIM_DIF_PARAM_FILE"
        }
    };

    jcli::JsonSpec spec{cli};
    spec.enable_scoping();


    double a = spec.get_option<double>(
            "diff-coef", 
            {"d", "D"}, 
            1000000, 
            "The diffusion coefficient in m/s^2"
            );

    unsigned b = spec.get_option<unsigned>(
            "seed", 
            {"s", "S"}, 
            0, 
            "Random seed",
            [](unsigned s) -> bool {return s<10;}
            );

    json relations = spec.get_option<json>(
            "relations", 
            {"r", "R"}, 
            json{},
            "A json document"
            );

    spec.enable_config();
    spec.enable_debug();
    spec.enable_help();

    std::cout << a << ", " << b << std::endl;

}


