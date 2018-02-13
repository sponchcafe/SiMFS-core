#ifndef PH2_EMISSION_ACTION_H
#define PH2_EMISSION_ACTION_H

#include "graph/rate_graph.hpp"
#include "sim/io.hpp"
#include "sim/types.hpp"
#include "random/random.hpp"
#include <iostream>

using namespace sim;

class EmissionAction : public graph::Action{

    public:

        EmissionAction(
                const std::string name,
                graph::Graph &graph,
                io::Output<io::timetag> &out,
                unsigned seed);

        void fire() override;
        void set_efficiency(const io::efficiency eff);

    private:
       
        io::efficiency efficiency = 0.0; 
        io::Output<io::timetag> &output;
        random::Uniform uniform;

};

#endif
