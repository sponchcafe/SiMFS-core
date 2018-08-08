#include "component/component.hpp"
#include "component/types.hpp"
#include "random/random.hpp"
#include <cmath>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Splitter : public Component{

            public:

                //-----------------------------------------------------------//
                Splitter ();

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
                void set_seed(unsigned s);
                void set_photon_input_id(std::string id);
                void set_efficiency_input_id(std::string id);
                void set_accepted_photon_output_id(std::string id);
                void set_rejected_photon_output_id(std::string id);
                //-----------------------------------------------------------//
               
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_photon_input(){
                    set_photon_input<InputT>(photon_input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT> 
                void set_photon_input(std::string id){
                    photon_input_id = id;
                    photon_input_ptr = 
                        create_input<InputT, realtime_t>(
                            photon_input_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_efficiency_input(){
                    set_efficiency_input<InputT>(efficiency_input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT> 
                void set_efficiency_input(std::string id){
                    efficiency_input_id = id;
                    efficiency_input_ptr = 
                        create_input<InputT, TimedValue>(
                            efficiency_input_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_accepted_photon_output(){
                    set_accepted_photon_output<OutputT>(accepted_photon_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_accepted_photon_output(std::string id){
                    accepted_photon_output_id = id;
                    accepted_photon_output_ptr = 
                        create_output<OutputT, realtime_t>(
                            accepted_photon_output_id
                        );
                }
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_rejected_photon_output(){
                    set_rejected_photon_output<OutputT>(rejected_photon_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_rejected_photon_output(std::string id){
                    rejected_photon_output_id = id;
                    rejected_photon_output_ptr = 
                        create_output<OutputT, realtime_t>(
                            rejected_photon_output_id
                        );
                }
                //-----------------------------------------------------------//
                
            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                unsigned seed = 0;
                std::string efficiency_input_id = "./efficiency";
                std::string photon_input_id = "./emission";
                std::string accepted_photon_output_id = "./accepted";
                std::string rejected_photon_output_id = "/dev/null";

                //-----------------------------------------------------------//
                random::Uniform uni{0};         // seed 0

                //-----------------------------------------------------------//
                realtime_t timetag{0.0};
                TimedValue efficiency{0.0, 0.0};

                //-----------------------------------------------------------//
                std::unique_ptr<Input<TimedValue>> efficiency_input_ptr;
                std::unique_ptr<Input<realtime_t>> photon_input_ptr;
                std::unique_ptr<Output<realtime_t>> accepted_photon_output_ptr;
                std::unique_ptr<Output<realtime_t>> rejected_photon_output_ptr;

        };

    }
}
