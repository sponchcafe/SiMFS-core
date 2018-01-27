#ifndef PH2_EMISSION_ACTION_H
#define PH2_EMISSION_ACTION_H

#include "graph/rate_graph.hpp"
#include "sim/io.hpp"
#include "sim/types.hpp"
#include <iostream>

using namespace sim;

class EmissionAction : public graph::Action{

    public:

        EmissionAction(
                const std::string name,
                graph::Graph &graph,
                io::Output<io::timetag> &out);

        void fire() override;

    private:
        
        io::Output<io::timetag> &output;

};

#endif
