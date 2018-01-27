#include "emission_action.hpp"

EmissionAction::EmissionAction(
        const std::string name,
        graph::Graph &graph,
        io::Output<io::timetag> &out) :
    Action(name, graph),
    output(out){
    }

void EmissionAction::fire(){
    output.put(graph.clock);
}
