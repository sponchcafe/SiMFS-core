#include "sim/io.hpp"
#include "sim/types.hpp"
#include "sim/options.hpp"
#include "focus/alpha_gauss.hpp"

using namespace sim::io;
using namespace sim::opt;
using namespace sim::focus;

const std::string helpmessage = 
R"(
Calculate flux density at coordinates x, y, z read from standard in.
TODO: Function description

Usage: exiAlpha [options] < coordinates > flux
    
    [exiAlpha]

    -i --input : Input file (coordinates), defaults to standard in.
    -o --output : Output file (flux), defaults to standard out.

    -x --waist-xy : 1/e^2 waist in xy-direction in nanometer.
    -y --waist-z : 1/e^2 waist in z-direction in nanometer.
    -l --lambda : Excitation wavelength in nanometer.
    -P --power : Excitation power in watt.
)";

typedef struct{
    std::string infile;
    std::string outfile;
    si_coord_t waist_xy;
    si_coord_t waist_z;
    double lambda;
    double power;
} ExiAlphaParams;

ExiAlphaParams params;

int main (int argc, char **argv){

    Parameters p{argc, argv, "exiAlpha"};

    params.infile = p.getOption('i', "input", sim::opt::empty);
    params.outfile = p.getOption('o', "output", sim::opt::empty);
    params.waist_xy = p.getOption('x', "waist-xy", 200e-9);
    params.waist_z = p.getOption('z', "waist-z", 600e-9);
    params.lambda = p.getOption('l', "lambda", 488e-9);
    params.power = p.getOption('P', "power", 50e-6);

    p.enableConfig();
    p.enableHelp(helpmessage);

    Input<Coordinate> input(params.infile);
    Output<flux> output(params.outfile);

    Coordinate c{0, 0, 0};
    SI_Coordinate sic{0.0, 0.0, 0.0};
    flux f{0.0};

    AlphaGauss focus = AlphaGauss(params.waist_xy, params.waist_z);

    while(input.get(c)){
        sic = c;
        f = focus.evaluate(sic) * params.power * params.lambda / (CONST_H * CONST_C);
        output.put(f);
    }
    
}
