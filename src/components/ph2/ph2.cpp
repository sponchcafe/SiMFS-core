#include "sim/io.hpp"
#include "sim/options.hpp"
#include "json/json.hpp"
#include "graph/rate_graph.hpp"
#include "emission_action.hpp"
#include "detection_action.hpp"
#include "excitation_action.hpp"

const std::string helpmessage = 
R"(
Info
Usage: ph2 [options]
)";

json jablonsky =
{
    {{"name", "exi"}, {"from", "S0"}, {"to", "S1"}, {"epsilon", 73000}},
    {{"name", "ic"},  {"from", "S1"}, {"to", "S0"}, {"rate", 1e+4}}, 
    {{"name", "emi"}, {"from", "S1"}, {"to", "S0"}, {"rate", 1e+10}},
    {{"name", "isc"}, {"from", "S1"}, {"to", "T1"}, {"rate", 1e+7}},  
    {{"name", "risc"},{"from", "T1"}, {"to", "S0"}, {"rate", 1e+7}}
};

json excitation =
{
    {{"target", "exi"}, {"input", ""}, {"increment", 1e-7}}
}; 

json detection = 
{
    {{"target", "emi"}, {"input", ""}, {"increment", 1e-7}, {"output", ""}}
};

int main (int argc, char *argv[]){

    unsigned seed = 0;
    
    sim::opt::Parameters p{argc, argv, "ph2"};
    jablonsky = p.getOption('j', "jablonsky", jablonsky);
    excitation = p.getOption('e', "excitation", excitation);
    detection = p.getOption('d', "detection", detection);
    seed = p.getOption('s', "seed", 0);
   
    p.enableConfig();
    p.enableHelp(helpmessage);


    for (auto it = jablonsky.begin(); it != jablonsky.end(); ++it){
        std::cout << it->at("name");
    }

    /*
    std::vector<std::string> states = jstates;
    std::vector<json> connections = jconnections;
    
    // IO
    sim::io::Input<sim::io::efficiency> det(det_filename);
    sim::io::Output<sim::io::timetag> output(out_filename);

    sim::graph::Graph g = sim::graph::Graph(seed);

    for (auto it = states.begin(); it < states.end(); ++it) g.add_node(*it);

    for (auto it = connections.begin(); it < connections.end(); ++it){

        json con = *it;
        g.add_edge(con["name"], con["from"], con["to"], con["rate"]);

        // Build excitation
        if (con["epsilon"].get<double>() > 0.0){
            sim::io::Input<sim::io::flux> exi(exi_filename);
            sim::graph::Action *a = new ExcitationAction(con["name"], g, exi, con["epsilon"], dwell_time, con["name"]);
            g.add_action(a);
        }

        // Build the detection
        if (con["emission"].get<bool>()){
            sim::graph::Action *a = new EmissionAction(con["name"], g, output);
            g.add_action(a);
            g.link_edge_action(con["name"], con["name"]);
        }

    }

    g.init("S0", "exi");
    g.traverse();
*/    
}
