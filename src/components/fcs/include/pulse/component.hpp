#pragma once

#include "component/component.hpp"
#include "definitions/types.hpp"

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
                void set_input_id(std::string id);
                void set_output_id(std::string id);
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
                std::string input_id = "__flux_cw__";
                std::string output_id = "__flux_pulse__";
                //-----------------------------------------------------------//
                
                static constexpr double DELTA_T = 1e-12;
                static constexpr double DELTA_F = 1e-12;

                //-----------------------------------------------------------//
                TimedValue flux_in{0.0, 0.0};
                TimedValue flux_current{0.0, 0.0};
                TimedValue flux_out{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<TimedValue>> input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> output_ptr;

        };
    }
}
