#include "graph/main.hpp"
#include "io/base_io.hpp"

namespace sim{
    namespace graph{

        class EmissionAction : public Action{

            //---------------------------------------------------------------//
            // The emission actions puts a timetag to its output whenever it
            // is triggered. The timetag is the current graph time.
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                EmissionAction(
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
                void set_photon_output_id(std::string id);
                void set_trigger_edge_id(std::string id);
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_photon_output(){
                    set_photon_output<OutputT>(photon_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_photon_output(std::string id){
                    photon_output_id = id;
                    photon_output_ptr = comp::create_output<OutputT, realtime_t>(
                            photon_output_id
                        );
                }

            private:

                //-----------------------------------------------------------//
                std::string photon_output_id = "__emission__";
                std::string trigger_edge_id = "emi";

                //-----------------------------------------------------------//
                std::unique_ptr<sim::comp::Output<realtime_t>> photon_output_ptr;

        };
    }
}
