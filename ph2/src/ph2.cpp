#include "sim_util.hpp"
#include "ph2_fluorophore.hpp"
#include <vector>

const std::string helpmessage = 
R"(
Create a 3D evenly spaced grid of coordinates written in binary 16-bit integers to the standard output.

Usage: grd [options]

    -x --x-points : Number of points in the x dimension.
    -y --y-points : Number of points in the y dimension.
    -z --z-points : Number of points in the z dimension.

    -X --x-limit : Maximal distance from origin in x in meters.
    -Y --y-limit : Maximal distance from origin in y in meters.
    -Z --z-limit : Maximal distance from origin in z in meters.

    -p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
    -c --config : Generate a config .json file and print it to the standard output.
    -h --help : Show this help message.
)";

json jablonsky =
        {
            {"states", {"S0", "S1", "T1"}},
            {"connections", {
                {{"from", "S0"}, {"to", "S1"}, {"rate", 0}   ,  {"epsilon", 73000}, {"emission", false}},
                {{"from", "S1"}, {"to", "S0"}, {"rate", 1e+4},  {"epsilon", 0},     {"emission", false}},
                {{"from", "S1"}, {"to", "S0"}, {"rate", 1e+10}, {"epsilon", 0},     {"emission", true}},
                {{"from", "S1"}, {"to", "T1"}, {"rate", 1e+7},  {"epsilon", 0},     {"emission", false}},
                {{"from", "T1"}, {"to", "S0"}, {"rate", 1e+7},  {"epsilon", 0},     {"emission", false}}
            }
            }
        };

int main (int argc, char *argv[]){

    double dwell_time;
    std::string exi_file = "./excitation";
    std::string det_file = "./detection";
    json jstates = jablonsky["states"];
    json jconnections = jablonsky["connections"];

    sim::opt::Parameters p{argc, argv};
	dwell_time = p.getOption('t', "dwell-time", 1e-7);
    exi_file = p.getOption('e', "excitation", exi_file);
	det_file = p.getOption('d', "detection", det_file);
	jstates = p.getOption('s', "states", jstates);
	jconnections = p.getOption('l', "connections", jconnections);
   
    p.enableConfig();
    p.enableHelp(helpmessage);

    std::vector<std::string> states = jstates;
    std::vector<json> connections = jconnections;

    ph2::fluorophore f{0, dwell_time, std::cout};
    for (auto it = states.begin(); it < states.end(); ++it) f.add_state(*it);
    
    for (auto it = connections.begin(); it < connections.end(); ++it){
        json con = *it;
        std::string from = con["from"];
        std::string to = con["to"];
        double rate = con["rate"];
        bool emission = con["emission"];
        double epsilon = con["epsilon"];
        f.connect_states(from, to, rate, emission, epsilon);
    }
    
    f.initialize(states[0]);

    std::ifstream exi(exi_file, std::ifstream::in);
    std::ifstream det(det_file, std::ifstream::in);

    sim::io::flux flux;
    sim::io::efficiency eff;

    while(sim::io::read_binary(exi, flux) && sim::io::read_binary(det, eff)){
        f.update_environment(flux, eff);
        f.simulate();
    }

    return 0;

}