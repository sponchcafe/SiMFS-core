#include "shellutils/shellutils.hpp"
#include "random/random.hpp"
#include <cmath>

const std::string helpmessage = R"(
Create a random diffusion path as coordinates written in binary 16-bit 
integers to the standard output. The coordinates are part of a cylindrical 
space.)";


double get_sigma (double diff_coef, double increment){
    return sqrt(2 * diff_coef * increment);
}

bool check_within_box (double x, double y, double z,
        double radius, double half_height){
    bool in_xy = sqrt(pow(x, 2)+pow(y, 2)) < radius;
    bool in_z = abs(z) < half_height;
    return in_xy && in_z;
}

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, 
        {"SIM_PARAMS", "SIM_DIF_PARAMS"}
    };
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();

    //----------------------------------------------------------------------//
    double increment = spec.get_option<double>
        (
         "increment", {"i", "I"}, 1e-7,
         "Time increment in seconds.",
         [](double val) -> bool {return val>0;}
        );

    //----------------------------------------------------------------------//
    double exp_time = spec.get_option<double>
        (
         "experiment-time", {"t", "T"}, 1.0,
         "Simulation time in seconds",
         [](double val) -> bool {return val>0;}
        );

    //----------------------------------------------------------------------//
    double time_offset = spec.get_option<double>
        (
         "time-offset", {"x", "X"}, 0.0,
         "Experiment start time in seconds.",
         [](double val) -> bool {return val>=0;}
        );

    //----------------------------------------------------------------------//
    std::string out_filename = spec.get_option<std::string>
        (
         "output", {"o", "O"}, jcli::EMPTY_STRING,
         "Output file (coordinates), defaults to stdout"
        );

    //----------------------------------------------------------------------//
    double radius = spec.get_option<double>
        (
         "radius", {"r", "R"}, 5e-7,
         "Radius of the diffusion cylinder in m.",
         [](double val) -> bool {return val>0;}
        );
    
    //----------------------------------------------------------------------//
    double half_height = spec.get_option<double>
        (
         "half-height", {"h", "H"}, 1e-6,
         "Half-height of the diffusion cylinder in m.",
         [](double val) -> bool {return val>0;}
        );
    
    //----------------------------------------------------------------------//
    double diff_coef = spec.get_option<double>
        (
         "diff-coef", {"d", "D"}, 1e-10,
         "Diffusion coefficient in m/s^2.",
         [](double val) -> bool {return val>0;}
        );

    //----------------------------------------------------------------------//
    unsigned seed = spec.get_option<unsigned>
        (
         "seed", {"s", "S"}, sim::random::get_new_seed(),
         "Random seed."
        );

    spec.enable_help(); // --help
    spec.enable_config(); // --config
    spec.enable_debug(); // --debug
    spec.enable_log(); // --log

    uint64_t steps = (uint64_t) floor(exp_time/increment);
    double sigma = get_sigma(diff_coef, increment); 

    sim::io::Output<sim::Coordinate> out(out_filename);
    sim::Coordinate c0{0.0,0.0,0.0,0.0};
    sim::Coordinate c1{0.0,0.0,0.0,0.0};
    sim::random::Normal rand{sigma, seed};

    // Initial position:
    sim::random::Uniform uni{seed};
    do{
        c0.x = (uni()*2 - 1)*radius;
        c0.y = (uni()*2 - 1)*radius;
        c0.z = (uni()*2 - 1)*half_height;
    } while(!check_within_box(c0.x, c0.y, c0.z, radius, half_height));

    uint64_t i = 0;
    while(i<steps){

        c1.x = c0.x + rand();
        c1.y = c0.y + rand();
        c1.z = c0.z + rand();

        if (check_within_box(c1.x, c1.y, c1.z, radius, half_height)){
            c0 = c1;
            c0.t = time_offset + increment * i;
            out.put(c0);
            i++;
        }
    }
    
}
