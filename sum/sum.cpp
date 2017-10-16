#include "sim_util.hpp"

const std::string helpmessage = 

R"(
Count photon timetags in fixed size bins.

Usage: sum [options] < photons > binned_timetrace

    -w --width : Width of time bins in seconds.

    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
    -c --config : Generate a config .json file and print it to the standard output.
    -h --help : Show this help message.
)";

double width = 1e-3;

int main(int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "sum"};
    width = p.getOption('w', "width", 1e-3);
   
    p.enableConfig();
    p.enableHelp(helpmessage);

    if (width <= 0){
        std::string execname = argv[0];
        sim::log::warn("["+execname+"] WARNING: Non-positive bin-widths are not allowed.\n");
    }
        
    sim::io::timetag t{0.0};
    sim::io::photon_count count = 0;
    double bin_end = width;

    while(sim::io::read_binary(std::cin, t)){
        if(t>=bin_end){
            sim::io::write_binary(std::cout, count);
            count = 0;
            bin_end += width;
        }else{
            count++;
        }
    }
    sim::io::write_binary(std::cout, count);
  
    return 0;

}
