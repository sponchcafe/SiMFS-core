#include "shellutils/shellutils.hpp"
#include <vector>
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

    std::string input_name = spec.get_option<std::string>(
            "input", 
            {"i", "I"}, 
            "", 
            "Input file"
            );
    std::string output_name = spec.get_option<std::string>(
            "output", 
            {"o", "O"}, 
            "", 
            "Output file"
            );


    spec.enable_config();
    spec.enable_debug();
    spec.enable_help();

    
    sim::io::Input<double> input(input_name);
    sim::io::Output<double> output(output_name);
    double d;

    std::vector<sim::io::Input<double>> inputs;
    inputs.push_back(std::move(input));

    while(inputs[0].get(d)){
        output.put(d);
    }


}


