#include "emission_action.hpp"

EmissionAction::EmissionAction(
        const std::string name,
        graph::Graph &graph,
        io::Output<io::timetag> &out,
        unsigned seed) : 
    Action(name, graph),
    output(out){
        uniform = random::Uniform(seed);
    }

void EmissionAction::fire(){
    if (uniform() < efficiency) output.put(graph.clock);
}

void EmissionAction::set_efficiency(const io::efficiency eff){
    efficiency = eff;
}

