#include "sim_util.hpp"

const std::string helpmessage = 
R"(
C

Usage: detAlpha [options] < coordinates > efficiency
    
    [detAlpha]

    -i --input : Input file (coordinates), defaults to standard in.
    -o --output : Output file (flux), defaults to standard out.

    -x --waist-xy : 1/e^2 waist in xy-direction in nanometer.
    -y --waist-z : 1/e^2 waist in z-direction in nanometer.

    -a --amplitude : Maximal amplitude of detection efficiency function.
)";

double waist_xy;
double waist_z;
double amplitude;


const double efficiency(sim::io::coordinate const &c){
    double x1 = pow(1e-9*c.z,2)/pow(waist_z,2);
    double x2 = (pow(1e-9*c.x, 2)+pow(1e-9*c.y, 2));
    double x3 = (exp(-2*x1))/pow(waist_xy, 2);
    double efficiency = amplitude * exp(-2*(x1+(x2*x3)));
    return efficiency;
}

int main (int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "detAlpha"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    waist_xy = p.getOption('x', "waist-xy", 100e-9);
    waist_z = p.getOption('z', "waist-z", 100e-9);
    amplitude = p.getOption('a', "amplitude", 1.0);
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Input<sim::io::coordinate> input(in_filename);
    sim::io::Output<sim::io::efficiency> output(out_filename);
        
    sim::io::coordinate c{0,0,0};
    sim::io::efficiency e{0.0};
    while(input.get(c)){
        e = efficiency(c);
        output.put(e);
    }
  
    return 0;

}
