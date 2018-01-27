#include "sim/io.hpp"
#include "sim/types.hpp"
#include "sim/options.hpp"
#include "random/random.hpp"
#include <cmath>

using namespace sim::io;
using namespace sim::random;
using namespace sim::opt;

const std::string helpmessage = 
R"(
Create a random diffusion path as coordinates written in binary 16-bit integers to the standard output.
The coordinates are part of a cylindrical space.

Usage: dif [options]
	
    -o --output : Output file (coordinates), defaults to standard out.

    -t --experiment-time : Simulation time.
    -i --increment : Time increment in seconds.

    -r --radius : Radius of the cylinder.
    -l --half-height : Half height of the cylinder.
    -d --diff-coef : Diffusion coefficient.
    
    -s --seed : Random seed.
)";

typedef struct {
    double increment;
    double ex_time;
    double diff_coef;
    unsigned seed;
    std::string out_filename;
    coord_t radius;
    coord_t half_height;
} DiffParams;

DiffParams params;
double get_sigma (){
    return sqrt(2 * params.diff_coef * params.increment);
}

bool check_within_box (Coordinate const &c){
    bool in_xy = sqrt(pow(c.x, 2)+pow(c.y, 2)) < params.radius;
    bool in_z = abs(c.z) < params.half_height;
    return in_xy && in_z;
}

int main(int argc, char **argv){

    Parameters p{argc, argv, "dif"};

    params.increment = p.getOption('i', "increment", 1e-7);
    params.ex_time = p.getOption('t', "experiment-time", 1.0);
    params.out_filename = p.getOption('o', "output", empty); 
    params.radius = (coord_t) (1e+9*p.getOption('r', "radius", 500e-9));
    params.half_height = (coord_t) (1e+9*p.getOption('l', "half-height", 1000e-9));
    params.diff_coef = p.getOption('d', "diff-coef", 1e+8);
    params.seed = p.getOption('s', "seed", 0);

    p.enableConfig();
    p.enableHelp(helpmessage);

    uint64_t steps = (uint64_t) floor(params.ex_time/params.increment);
    double sigma = get_sigma(); 

    Output<Coordinate> out(params.out_filename);
    Coordinate c0{0, 0, 0};
    Coordinate c1{0, 0, 0};
    Normal rand{sigma, params.seed};

    // Initial position:
    UniformInt init_z{-params.half_height, params.half_height, params.seed};
    UniformInt init_x{-params.radius, params.radius, params.seed+1};
    UniformInt init_y{-params.radius, params.radius, params.seed+2};
    
    do{
        c0.x = init_x();
        c0.y = init_y();
        c0.z = init_z();
    } while(!check_within_box(c0));

    for (int i=0; i<steps; i++){
        c1.x = c0.x + rand();
        c1.y = c0.y + rand();
        c1.z = c0.z + rand();
        if (check_within_box(c1)) c0 = c1;
        out.put(c0);
    }
    
}
