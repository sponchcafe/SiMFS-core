#pragma once

#include "component/component.hpp"
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
                void set_heartbeat(bool hb);
                void set_photon_input_id(std::string id);
                void set_efficiency_input_id(std::string id);
                void set_accepted_photon_output_id(std::string id);
                void set_rejected_photon_output_id(std::string id);
                //-----------------------------------------------------------//
               
            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                bool heartbeat = false;
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
                std::unique_ptr<io::BufferInput<TimedValue>> efficiency_input_ptr;
                std::unique_ptr<io::BufferInput<realtime_t>> photon_input_ptr;
                std::unique_ptr<io::BufferOutput<realtime_t>> accepted_photon_output_ptr;
                std::unique_ptr<io::BufferOutput<realtime_t>> rejected_photon_output_ptr;

        };

    }
}
