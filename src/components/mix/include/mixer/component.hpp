#include "component/component.hpp"
#include "component/types.hpp"
#include <vector>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Mixer : public Component{

            public:

                //-----------------------------------------------------------//
                Mixer ();

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
                void set_photon_output_id(std::string id);
                void set_photon_input_ids(std::vector<std::string> ids);
                //-----------------------------------------------------------//
               
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_photon_output(){
                    set_photon_output<OutputT>(photon_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT> 
                void set_photon_output(std::string id){
                    photon_output_id = id;
                    photon_output_ptr = 
                        create_output<OutputT, realtime_t>(
                            photon_output_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_photon_inputs(){
                    set_photon_inputs<InputT>(photon_input_ids);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_photon_inputs(std::vector<std::string> ids){
                    photon_input_ids = ids;
                    photon_input_ptrs.clear();
                    for (auto &it: photon_input_ids){
                        std::unique_ptr<Input<realtime_t>> photon_input_ptr = 
                            create_input<InputT, realtime_t>(it);
                        photon_input_ptrs.push_back(std::move(photon_input_ptr));
                    }
                }
                //-----------------------------------------------------------//

            private:

                //-----------------------------------------------------------//
                void sort_inputs();

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                std::string photon_output_id = "./mixed";
                std::vector<std::string> photon_input_ids{"./photons"};

                //-----------------------------------------------------------//
                realtime_t current{0.0}; 

                //-----------------------------------------------------------//
                std::unique_ptr<Output<realtime_t>> photon_output_ptr;
                std::vector<std::unique_ptr<Input<realtime_t>>> photon_input_ptrs;

        };

    }
}
