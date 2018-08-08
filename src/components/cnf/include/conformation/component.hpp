#pragma once

#include "component/component.hpp"
#include "component/types.hpp"
#include "graph/main.hpp"
#include "io/buffer.hpp"
#include <cmath>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Conformation : public Component{

            public:

                //-----------------------------------------------------------//
                Conformation ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//


                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_value_output_id(std::string id);
                void set_initial_state_id(std::string id);
                void set_seed(unsigned s);
                void set_experiment_time(realtime_t t);
                void set_states(json j);
                void set_values(json j);
                //-----------------------------------------------------------//
               

            private:

                //-Helpers---------------------------------------------------//
                void add_edge(std::string id, json j);

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                unsigned seed = 0;
                std::string value_output_id = "__value_output__";
                std::string initial_state_id = "N0";
                realtime_t experiment_time = 1.0;
                json states = {
                    {"N0->N1", {{"from", "N0"}, {"to", "N1"}, {"rate", 1e+2}}},
                    {"N1->N0", {{"from", "N1"}, {"to", "N0"}, {"rate", 1e+2}}}
                };
                json values = {
                    {"N0", 0.3},
                    {"N1", 0.7}
                };

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferOutput<TimedValue>> value_output_ptr;
                std::unique_ptr<graph::Graph> graph;


        };

    }
}
