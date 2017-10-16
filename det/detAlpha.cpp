#include "sim_util.hpp"
#include <cmath>

const std::string helpmessage = 

R"(
Calculate the detection efficiency at coordinates x, y, z from stdandard in.
The detection function is a 3D gauss with 1/e^2 waists u, v, w and offset x, y, z normalized in each xy-plane and with respect to the amplitude at (0,0,0).

Usage: detGauss [options] < coordinates > efficiency

    -u --waist-x : 1/e^2 waist in x-direction in meters.
    -v --waist-y : 1/e^2 waist in y-direction in meters.
    -w --waist-z : 1/e^2 waist in z-direction in meters.
    -x --offset-x : Offset in x-direction in meters.
    -y --offset-y : Offset in y-direction in meters.
    -z --offset-z : Offset in z-direction in meters.
    -a --amplitude : Maximal amplitude of detection efficiency function [0,1].

    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
    -c --config : Generate a config .json file and print it to the standard output.
    -h --help : Show this help message.
)";

const double MAX_AMP = 63661977236758.16; // static value of efficiency(0,0,0)

double offset_x;
double offset_y;
double offset_z;
double waist_xy;
double waist_z;
double amplitude;

double efficiency(sim::io::coordinate &c){
    double x1 = pow(1/(sqrt(CONST_PI/2) * waist_xy), 2);
    double x2 = pow(1e-9*c.z,2)/pow(waist_z,2);
    double x3 = (pow(1e-9*c.x, 2)+pow(1e-9*c.y, 2));
    double x4 = (exp(-2*x2))/pow(waist_xy, 2);
    double detection = x1 * exp(-2*(x2+(x3*x4))) / MAX_AMP;
    return detection;
}

int main(int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "det"};
    waist_xy = p.getOption('v', "waist-xy", 100e-9);
    waist_z = p.getOption('w', "waist-z", 100e-9);
    offset_x = p.getOption('x', "offset-x", 0.0);
    offset_y = p.getOption('y', "offset-y", 0.0);
    offset_z = p.getOption('z', "offset-z", 0.0);
    amplitude = p.getOption('a', "amplitude", 1.0);
    p.enableConfig();
    p.enableHelp(helpmessage);

    if (amplitude < 0 || amplitude > 1){
        std::string execname = argv[0];
        sim::log::warn("["+execname+"] WARNING: Amplitude not in range [0,1].\n");
    }
    
        
    sim::io::coordinate c{0,0,0};
    while(sim::io::read_binary(std::cin, c)){
        sim::io::write_binary(std::cout, efficiency(c));
    }
  
    return 0;

}
