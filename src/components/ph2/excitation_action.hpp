#ifndef PH2_EXCITATION_ACTION_H
#define PH2_EXCITATION_ACTION_H

#include "graph/rate_graph.hpp"
#include "sim/io.hpp"
#include "sim/types.hpp"
#include <iostream>

using namespace sim;

class ExcitationAction : public graph::Action{
    
    /* The excitation action reads a flux value from an input stream,
     * calculates the excitation rate for its target edge according
     * to the specified absorption coefficient and wavelength. It updates
     * the edge lambda and issues a new update event a time increment later
     * until the input stream ends. It then switches the graph's done flag
     * to true. */
    
    public:

        ExcitationAction(
                const std::string name,
                graph::Graph &graph,
                io::Input<io::flux> &in,
                double abs_coef,
                double increment,
                const std::string target_edge_name);

        void init() override;
        void fire() override;

    private:

        const std::string target_edge_name;
        const double absorption_coefficient = 0.0;
        const double increment = 0.0;
        double scaling = 0.0;
        io::Input<io::flux> &input;
        graph::Edge *target = nullptr;

};

#endif
