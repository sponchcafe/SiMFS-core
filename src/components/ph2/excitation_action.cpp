#include "excitation_action.hpp"
#include <cmath>

ExcitationAction::ExcitationAction(
        const std::string name,
        graph::Graph &graph,
        io::Input<io::flux> &in,
        double abs_coef,
        double increment,
        const std::string target_edge_name) :
    Action(name, graph),
    target_edge_name(target_edge_name),
    absorption_coefficient(abs_coef),
    increment(increment),
    input(in){
    }

void ExcitationAction::init(){
    target = graph.get_edge(target_edge_name);
    scaling = absorption_coefficient * 100.0/1000 * log(10) / CONST_NA;
}

void ExcitationAction::fire(){
    /* main logic */
    double flux = 0.0;
    if(!input.get(flux)){
        graph.done = true;
        return;
    }
    target->set_lambda(scaling * flux);
    graph.push_event(graph::Event(this, graph.clock+increment));
}

