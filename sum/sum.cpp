#include "sim_util.hpp"

const std::string helpmessage = 

R"(
Count photon timetags in fixed size bins.

Usage: sum [options] < timetags > binned_timetrace

    -i --input : Input file (timetags), defaults to standard in.
    -o --output : Output file (counts), defaults to standard out.

    [sum]
    -w --width : Width of time bins in seconds.
)";

double width = 1e-3;

int main(int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "sum"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    width = p.getOption('w', "width", 1e-3);
   
    p.enableConfig();
    p.enableHelp(helpmessage);

    // IO
    sim::io::Input<sim::io::timetag> input(in_filename);
    sim::io::Output<sim::io::photon_count> output(out_filename, 32);
        
    sim::io::timetag t{0.0};
    sim::io::photon_count count = 0;
    double bin_end = width;

    while(input.get(t)){
        if(t>=bin_end){
            output.put(count);
            count = 0;
            bin_end += width;
        }
        count++;
    }
    output.put(count);
  
    return 0;

}
