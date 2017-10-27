#include "sim_util.hpp"
#include <cmath>

const std::string helpmessage = 

R"(
Calculate the detection efficiency at coordinates x, y, z from stdandard in.
The detection function is a 3D gauss with 1/e^2 waists u, v, w and offset x, y, z.

Usage: detGauss [options] < coordinates > efficiency

    -i --input : Input file (coordinates), defaults to stdandard in.
    -o --output : Output file (efficiency), defaults to standard out.

    [detGauss]
    -u --waist-x : 1/e^2 waist in x-direction in meters.
    -v --waist-y : 1/e^2 waist in y-direction in meters.
    -w --waist-z : 1/e^2 waist in z-direction in meters.

    -x --offset-x : Offset in x-direction in meters.
    -y --offset-y : Offset in y-direction in meters.
    -z --offset-z : Offset in z-direction in meters.

    -a --amplitude : Maximal amplitude of detection efficiency function.
)";

double waist_x;
double waist_y;
double waist_z;
double offset_x; 
double offset_y;
double offset_z;
double amplitude;

double efficiency(sim::io::coordinate &c){
    double x = exp(-4*pow(c.x*1e-9 - offset_x, 2)/(2*pow(waist_x, 2)));
    double y = exp(-4*pow(c.y*1e-9 - offset_y, 2)/(2*pow(waist_y, 2)));
    double z = exp(-4*pow(c.z*1e-9 - offset_z, 2)/(2*pow(waist_z, 2)));
    return x*y*z*amplitude;
}

int main(int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "detGauss"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    waist_x = p.getOption('u', "waist-x", 100e-9);
    waist_y = p.getOption('v', "waist-y", 100e-9);
    waist_z = p.getOption('w', "waist-z", 100e-9);
    offset_x = p.getOption('x', "offset-x", 0.0);
    offset_y = p.getOption('y', "offset-y", 0.0);
    offset_z = p.getOption('z', "offset-z", 0.0);
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
