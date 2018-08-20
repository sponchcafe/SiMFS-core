#pragma once

#include "component/component.hpp"
#include "component/types.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Pulse : public Component{

            public:

                //-----------------------------------------------------------//
                Pulse ();

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_flux_input_id(std::string id);
                void set_flux_output_id(std::string id);
                void set_pulse(double rep, std::vector<double> t, std::vector<double> a);
                //-----------------------------------------------------------//
                 
                
            private:

                //-----------------------------------------------------------//
                void normalize_pulse();

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double repetition_time = 1e-7;                          // s
                std::vector<double> pulse_time{0.0};                    // s
                std::vector<double> pulse_power{1.0};               // rel
                std::string flux_input_id = "__flux_cw__";
                std::string flux_output_id = "__flux_pulses__";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                TimedValue flux{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<TimedValue>> flux_input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> flux_output_ptr;

        };
    }
}
