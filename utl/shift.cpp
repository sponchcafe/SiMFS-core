#include "sim_util.hpp"

const std::string helpmessage = 
R"(
Shift coordinates in x, y, z.
Usage: shift [options] < coordinates > coordinates
    
    [exiAlpha]

    -i --input : Input file (coordinates), defaults to standard in.
    -o --output : Output file (flux), defaults to standard out.

    -x --offset-x : Offset in x-direction in meters.
    -y --offset-y : Offset in y-direction in meters.
    -z --offset-z : Offset in z-direction in meters.

)";

sim::io::coordinate offset;

int main (int argc, char *argv[]){
  
    sim::opt::Parameters p{argc, argv, "detGauss"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    offset.x = p.getOption('x', "offset-x", 0.0);
    offset.y = p.getOption('y', "offset-y", 0.0);
    offset.z = p.getOption('z', "offset-z", 0.0);
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Input<sim::io::coordinate> input(in_filename);
    sim::io::Output<sim::io::coordinate> output(out_filename);

    sim::io::coordinate c{0, 0, 0};
    while(input.get(c)){
        c.x += offset.x;
        c.y += offset.y;
        c.z += offset.z;
        output.put(c);
    }
    
}