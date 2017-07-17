#include "sim_util.hpp"

const std::string helpmessage = 
R"(
Calculate flux density at coordinates x, y, z read from standard in.
TODO: Function description

Usage: exiAlpha [options] < coordinates > flux

    -x --waist-xy : 1/e^2 waist in xy-direction in nanometer.
    -y --waist-z : 1/e^2 waist in z-direction in nanometer.
    -l --lambda : Excitation wavelength in nanometer.
    -P --power : Excitation power in watt.

    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
    -c --config : Generate a config .json file and print it to the standard output.
    -h --help : Show this help message.
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

    sim::opt::Parameters p{argc, argv};
	waist_xy = p.getOption('x', "waist-xy", 200e-9);
	waist_z = p.getOption('z', "waist-z", 600e-9);
	lambda = p.getOption('l', "lambda", 488e-9);
	power = p.getOption('P', "power", 50e-6);
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::coordinate c{0, 0, 0};
    while(sim::io::read_binary(std::cin, c)){
        sim::io::write_binary(std::cout, excitation(c));
    }
    
}