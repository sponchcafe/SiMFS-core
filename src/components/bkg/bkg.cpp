#include "shellutils/shellutils.hpp"
#include "random/random.hpp"

const std::string helpmessage = 
R"(
Info
Usage: bkg [options]
)";


int main (int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, {"SIM_PARAMS", "SIM_BKG_PARAMS"} };
    jcli::JsonSpec spec{cli};
    spec.enable_scoping();  // --scope

    //----------------------------------------------------------------------//
    unsigned seed = spec.get_option<unsigned>(
            "seed", {"s", "S"}, sim::random::get_new_seed(),
            "Random seed."
            );

    //----------------------------------------------------------------------//
    double time = spec.get_option<double>(
            "time", {"t", "T"}, 1.0,
            "Random seed."
            );

    //----------------------------------------------------------------------//
    double rate = spec.get_option<double>(
            "rate", {"r", "R"}, 1000,
            "Background rate per second"
            );

    //-----------------------------------------------------------------------//
    std::string output_name = spec.get_option<std::string>(
            "output", {"o", "O"}, "", 
            "Output"
            );


    spec.enable_help();
    spec.enable_config();
    spec.enable_log();
    spec.enable_debug();

    sim::io::Output<sim::realtime_t> output{output_name};
    sim::random::Exponential exponential = sim::random::Exponential(rate, seed);

    sim::realtime_t current = exponential();

    while(current < time){
        output.put(current);
        current += exponential();
    }


}
