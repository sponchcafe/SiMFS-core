#include "sim_util.hpp"
#include <random>

const std::string helpmessage = 
R"(
Generate a stream of background photons with a constant rate.

Usage: grd [options]
    
    -o --output : Output file, defaults to standard out.

    [GLOBAL] 
    -t --time : Experiment time.

    [grd]
    -r --rate : Mean photon rate.
    -s --seed : Random seed.
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

    sim::opt::Parameters globals{argc, argv, "GLOBAL"};
    double ex_time = globals.getOption('t', "experiment-time", 1.0);

    sim::opt::Parameters p{argc, argv, "bkg"};
    double rate = p.getOption('r', "rate", 1000.0);
    unsigned seed = p.getOption('s', "seed", 0);
    std::string outfile_name = p.getOption('o', "out", sim::opt::empty);
    
    sim::io::Output<sim::io::timetag> output(outfile_name);

    globals.enableConfig(false);
    p.enableConfig();
    p.enableHelp(helpmessage);

    random_engine = std::mt19937 (seed);

    sim::io::timetag ttag = get_lifetime(rate);

    while(ttag < ex_time){
        output.put(ttag);
        ttag += get_lifetime(rate);
    }

    return 0;

}