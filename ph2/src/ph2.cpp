#include "sim_util.hpp"
#include "ph2_fluorophore.hpp"
#include <vector>

const std::string helpmessage = 
R"(
Info

Usage: ph2 [options]

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
    unsigned int seed;
    std::string exi_default = "./exi";
    std::string det_default = "./det";
    json jstates = jablonsky["states"];
    json jconnections = jablonsky["connections"];

    
    sim::opt::Parameters p{argc, argv, "ph2"};
    dwell_time = p.getOption('i', "increment", 1e-7);
    std::string exi_filename = p.getOption('e', "excitation", exi_default);
    std::string det_filename = p.getOption('d', "detection", det_default);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
	jstates = p.getOption('v', "states", jstates);
	jconnections = p.getOption('u', "connections", jconnections);
    seed = p.getOption('s', "seed", 0);
   
    p.enableConfig();
    p.enableHelp(helpmessage);

    std::vector<std::string> states = jstates;
    std::vector<json> connections = jconnections;
    
    // IO
    sim::io::Input<sim::io::flux> exi(exi_filename);
    sim::io::Input<sim::io::efficiency> det(det_filename);
    sim::io::Output<sim::io::timetag> output(out_filename);

    ph2::fluorophore f{seed, dwell_time, output};
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

    sim::io::flux flux = 0.0;
    sim::io::efficiency eff = 0.0;

    while(exi.get(flux) && det.get(eff)){
        f.update_environment(flux, eff);
        f.simulate();
    }

    return 0;

}