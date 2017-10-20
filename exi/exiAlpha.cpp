#include "sim_util.hpp"

const std::string helpmessage = 
R"(
Calculate flux density at coordinates x, y, z read from standard in.
TODO: Function description

Usage: exiAlpha [options] < coordinates > flux

    -i --input : Input file (coordinates), defaults to standard in.
    -o --output : Output file (flux), defaults to standard out.

    [exiAlpha]
    -x --waist-xy : 1/e^2 waist in xy-direction in nanometer.
    -y --waist-z : 1/e^2 waist in z-direction in nanometer.
    -l --lambda : Excitation wavelength in nanometer.
    -P --power : Excitation power in watt.
)";

double waist_xy;
double waist_z;
double lambda;
double power;

const double excitation(sim::io::coordinate const &c){
    double x1 = pow(1/(sqrt(CONST_PI/2) * waist_xy), 2);
    double x2 = pow(1e-9*c.z,2)/pow(waist_z,2);
    double x3 = (pow(1e-9*c.x, 2)+pow(1e-9*c.y, 2));
    double x4 = (exp(-2*x2))/pow(waist_xy, 2);
    double flux_density = x1 * exp(-2*(x2+(x3*x4))) * power * lambda / (CONST_H * CONST_C);
    return flux_density;
}

int main (int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "exiAlpha"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
	waist_xy = p.getOption('x', "waist-xy", 200e-9);
	waist_z = p.getOption('z', "waist-z", 600e-9);
	lambda = p.getOption('l', "lambda", 488e-9);
	power = p.getOption('P', "power", 50e-6);
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Input<sim::io::coordinate> input(in_filename);
    sim::io::Output<sim::io::flux> output(out_filename);

    sim::io::coordinate c{0, 0, 0};
    sim::io::flux f{0.0};
    while(input.get(c)){
        f = excitation(c);
        output.put(f);
    }
    
}