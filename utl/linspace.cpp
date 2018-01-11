#include "sim_util.hpp"

const std::string helpmessage = 
R"(
TODO;
    [linspace]

    -o --output : Output file (flux), defaults to standard out.

    -a --start : Start value.
    -b --stop : End value.
    -s --step : Step value.
)";
int main (int argc, char *argv[]){
   
    sim::opt::Parameters p{argc, argv, "linspace"};
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
	double start = p.getOption('a', "start", 0.0);
	double stop = p.getOption('b', "stop", 1.0);
	double step = p.getOption('s', "step", 0.1);
	
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Output<sim::io::timetag> output(out_filename);
    sim::io::timetag tag = start;
  
    while (tag < stop){
        output.put(tag);
        tag += step;
    }

  }