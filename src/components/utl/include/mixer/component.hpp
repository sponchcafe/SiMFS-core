#pragma once

#include "component/component.hpp"
#include "io/buffer.hpp"
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
                std::unique_ptr<io::BufferOutput<realtime_t>> photon_output_ptr;
                std::vector<std::unique_ptr<io::BufferInput<realtime_t>>> photon_input_ptrs;

        };

    }
}
