#include "component/component.hpp"
#include "component/types.hpp"
#include "graph/main.hpp"
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
               
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_value_output(){
                    set_value_output<OutputT>(value_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT> 
                void set_value_output(std::string id){
                    value_output_id = id;
                    value_output_ptr = 
                        create_output<OutputT, TimedValue>(
                            value_output_id
                        );
                }


            private:

                //-Helpers---------------------------------------------------//
                void add_edge(std::string id, json j);

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                unsigned seed = 0;
                std::string value_output_id = "./fret_efficiency";
                std::string initial_state_id = "FO";
                realtime_t experiment_time = 1.0;
                json states = {
                    {"open", {{"from", "FC"}, {"to", "FO"}, {"rate", 1e+2}}},
                    {"close", {{"from", "FO"}, {"to", "FC"}, {"rate", 1e+2}}}
                };
                json values = {
                    {"FO", {{"node", "FO"}, {"value", 0.3}}},
                    {"FC", {{"node", "FC"}, {"value", 0.7}}}
                };

                //-----------------------------------------------------------//
                std::unique_ptr<Output<TimedValue>> value_output_ptr;
                graph::Graph graph;


        };

    }
}
