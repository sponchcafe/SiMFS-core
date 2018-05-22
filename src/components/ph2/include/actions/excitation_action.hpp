#include "graph/main.hpp" 
#include "io/base_io.hpp"

namespace sim{
    namespace graph{

        class ExcitationAction : public Action{

            //---------------------------------------------------------------//
            // The excitation action reads a flux value from an input stream,
            // calculates the excitation rate for its target edge according to
            // the specified absorption coefficient and wavelength. It updates
            // the edge lambda and issues a new update event a time increment
            // later until the input stream ends. It then switches the graph's
            // done flag to true.
            //---------------------------------------------------------------//

            public:

                static std::string type;

                //-----------------------------------------------------------//
                ExcitationAction(
                        std::string const name,
                        sim::graph::Graph &graph
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void set_target_edge_id(std::string id);
                void set_flux_input_id(std::string id);
                void set_absorption_coefficient(double epsilon);
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_flux_input(){
                    set_flux_input<InputT>(flux_input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_flux_input(std::string id){
                    flux_input_id = id;
                    flux_input_ptr = comp::create_input<InputT, TimedValue>(
                            flux_input_id
                        );
                }

            private:

                //-----------------------------------------------------------//
                std::string target_edge_id = "exi";
                std::string flux_input_id = "./excitation";
                double absorption_coefficient = 0.0;


                //-----------------------------------------------------------//
                double scaling = 0.0;
                std::unique_ptr<comp::Input<TimedValue>> flux_input_ptr;
                sim::graph::Edge *target = nullptr;
                sim::TimedValue current{0.0, 0.0};

        };

    }
}
