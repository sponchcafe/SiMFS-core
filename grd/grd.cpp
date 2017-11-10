#include "sim_util.hpp"

const std::string helpmessage = 
R"(
Create a 3D evenly spaced grid of coordinates written in binary 16-bit integers to the standard output.

Usage: grd [options]

    [grd]
    
    -o --output : Output file (coordinates), defaults to standard out.

    -x --x-points : Number of points in the x dimension.
    -y --y-points : Number of points in the y dimension.
    -z --z-points : Number of points in the z dimension.

    -X --x-limit : Maximal distance from origin in x in meters.
    -Y --y-limit : Maximal distance from origin in y in meters.
    -Z --z-limit : Maximal distance from origin in z in meters.
)";

int main (int argc, char *argv[]){

    int16_t x, y, z, X, Y, Z;

    sim::opt::Parameters p{argc, argv, "grd"};
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
	x = p.getOption('x', "x-points", 100);
	y = p.getOption('y', "y-points", 100);
	z = p.getOption('z', "z-points", 100);
	X = (int16_t) (1e+9*p.getOption('X', "x-limit", 1e-6));
	Y = (int16_t) (1e+9*p.getOption('Y', "y-limit", 1e-6));
	Z = (int16_t) (1e+9*p.getOption('Z', "z-limit", 1e-6));
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Output<sim::io::coordinate> output(out_filename);
    sim::io::coordinate c{0, 0, 0};

    for (int k=0; k<x; k++){
        for (int j=0; j<y; j++){
            for (int i=0; i<z; i++){
                c.x = 2*X/x*k-X;
                c.y = 2*Y/y*j-Y;
                c.z = 2*Z/z*i-Z;
                output.put(c);
            }
        }
    }

}