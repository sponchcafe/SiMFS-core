#include "sim/io.hpp"
#include "sim/options.hpp"
#include "json/json.hpp"
#include "graph/rate_graph.hpp"
#include "emission_action.hpp"
#include "excitation_action.hpp"

const std::string helpmessage = 
R"(
Info

Usage: ph2 [options]

)";

json jablonsky =
{
    {"states", 
        {"S0", "S1", "T1"}
    },
    {"connections", 
        {
            {{"name", "exi"}, {"from", "S0"}, {"to", "S1"}, {"rate", 0}   ,  {"epsilon", 73000}, {"emission", false}},
            {{"name", "ic"},  {"from", "S1"}, {"to", "S0"}, {"rate", 1e+4},  {"epsilon", 0},     {"emission", false}},
            {{"name", "emi"}, {"from", "S1"}, {"to", "S0"}, {"rate", 1e+10}, {"epsilon", 0},     {"emission", true}},
            {{"name", "isc"}, {"from", "S1"}, {"to", "T1"}, {"rate", 1e+7},  {"epsilon", 0},     {"emission", false}},
            {{"name", "risc"},{"from", "T1"}, {"to", "S0"}, {"rate", 1e+7},  {"epsilon", 0},     {"emission", false}}
        }
    }
};

int main (int argc, char *argv[]){

    double dwell_time = 0.0;
    unsigned int seed = 0;
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

    sim::graph::Graph g = sim::graph::Graph(seed);

    for (auto it = states.begin(); it < states.end(); ++it) g.add_node(*it);

    for (auto it = connections.begin(); it < connections.end(); ++it){

        json con = *it;
        g.add_edge(con["name"], con["from"], con["to"], con["rate"]);

        if (con["epsilon"].get<double>() > 0.0){
            sim::graph::Action *a = new ExcitationAction(con["name"], g, exi, con["epsilon"], dwell_time, con["name"]);
            g.add_action(a);
        }

        if (con["emission"].get<bool>()){
            sim::graph::Action *a = new EmissionAction(con["name"], g, output);
            g.add_action(a);
            g.link_edge_action(con["name"], con["name"]);
        }

    }

    sim::io::Output<sim::io::timetag> triplet("triplet.dat");
    sim::graph::Action *a = new EmissionAction("triplet", g, triplet);
    g.add_action(a);
    g.link_edge_action("triplet", "isc");
    g.link_edge_action("triplet", "risc");

    g.init("S0", "exi");
    g.traverse();
    
}
