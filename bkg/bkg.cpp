#include "sim_util.hpp"
#include <random>

const std::string helpmessage = 
R"(
Generate a stream of background photons with a constant rate.

Usage: grd [options]

    -t --time : [GLOBAL] Experiment time.

    -r --rate : Mean photon rate.
    -s --seed : Random seed.

    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
    -c --config : Generate a config .json file and print it to the standard output.
    -h --help : Show this help message.
)";

/* Function copied from ph2 ... */

std::mt19937 random_engine;

const double get_lifetime(double lambda)
{
    if (lambda <= 0){
        return std::numeric_limits<double>::max();
    }

    double y = ( (double) random_engine() / (double) random_engine.max());
    return -log(1-y) / lambda;
}

int main (int argc, char *argv[]){

    double rate;
    double ex_time;
    unsigned seed;
    

    sim::opt::Parameters globals{argc, argv, "GLOBAL"};
    ex_time = globals.getOption('t', "experiment-time", 1.0);

    sim::opt::Parameters p{argc, argv, "bkg"};
    rate = p.getOption('r', "rate", 1000.0);
    seed = p.getOption('s', "seed", 0);
    
    globals.enableConfig(false);
    p.enableConfig();
    p.enableHelp(helpmessage);

    random_engine = std::mt19937 (seed);

    sim::io::timetag ttag = get_lifetime(rate);

    while(ttag < ex_time){
        sim::io::write_binary(std::cout, ttag);
        ttag += get_lifetime(rate);
    }

    return 0;

}