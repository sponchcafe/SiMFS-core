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
                void set_heartbeat(bool hb);
                void set_routed(bool r);
                void set_photon_output_id(std::string id);
                void set_photon_input_ids(std::vector<std::string> ids);
                //-----------------------------------------------------------//

            private:

                typedef struct {
                    unsigned short route;
                    std::unique_ptr<io::BufferInput<realtime_t>> input_ptr;
                } RoutedInput;

                //-----------------------------------------------------------//
                void put_timetag(RoutedTime tag);

                //-----------------------------------------------------------//
                void sort_inputs();

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                bool heartbeat = false;
                bool routed = false;
                std::string photon_output_id = "__mixed__";
                std::vector<std::string> photon_input_ids{"__tags1__"};

                //-----------------------------------------------------------//
                RoutedTime current{0.0, 0}; 

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferOutput<realtime_t>> single_channel_photon_output_ptr;
                std::unique_ptr<io::BufferOutput<RoutedTime>> routed_photon_output_ptr;

                std::vector<RoutedInput> photon_input_ptrs;

        };

    }
}
