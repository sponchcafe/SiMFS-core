#include "graph/main.hpp"
#include "io/base_io.hpp"

namespace sim{
    namespace graph{

        class TransferAction : public Action{

            //---------------------------------------------------------------//
            // The transfer action consumes photon timetags from its input and
            // issues excitation events for its target edge. When the event is
            // fired and the graph is in an excitable state, an excitation state
            // transtition is performed. If the graph is however not in an
            // excitable state, the photon transfer is rejected and the events
            // timetag is put to the rejected output and can be mixed back into
            // the donor emission stream.
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                TransferAction(
                        std::string const name,
                        sim::graph::Graph &graph);

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
                void set_source_node_id(std::string id);
                void set_photon_input_id(std::string id);
                void set_rejected_photon_output_id(std::string id);
                //-----------------------------------------------------------//
                
                sim::graph::Graph &get_graph();

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_photon_input(){
                    set_photon_input<InputT>(photon_input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_photon_input(std::string id){
                    photon_input_id = id;
                    photon_input_ptr = comp::create_input<InputT, realtime_t>(
                            photon_input_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_rejected_photon_output(){
                    set_rejected_photon_output<OutputT>(rejected_photon_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_rejected_photon_output(std::string id){
                    rejected_photon_output_id = id;
                    rejected_photon_output_ptr = comp::create_output<OutputT, realtime_t>(
                            rejected_photon_output_id
                        );
                }

            private:

                //-----------------------------------------------------------//
                std::string target_edge_id = "exi";
                std::string source_node_id = "S0";
                std::string photon_input_id = "__transferred__";
                std::string rejected_photon_output_id = "__rejected__";

                //-----------------------------------------------------------//
                Edge *target_edge_ptr = nullptr;
                Node *source_node_ptr = nullptr;
                std::unique_ptr<comp::Input<realtime_t>> photon_input_ptr;
                std::unique_ptr<comp::Output<realtime_t>> rejected_photon_output_ptr;

                realtime_t current; 
        };

    }
}
