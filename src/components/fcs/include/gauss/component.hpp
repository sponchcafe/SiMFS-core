#pragma once

#include "component/component.hpp"
#include "focus/gauss.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class FCS_Gauss : public Component{

            public:

                //-----------------------------------------------------------//
                FCS_Gauss ();

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
                void set_output_id(std::string id);
                void set_input_id(std::string id);
                void set_wavelength(double w);
                void set_power(double p);
                void set_waists(double w_x, double w_y, double w_z);
                void set_mode(FocusMode m);
                //-----------------------------------------------------------//
                 
                
            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double wavelength = 488e-9;                             // m
                double power = 10e-6;                                   // W
                double waist_x = 200e-9;
                double waist_y = 200e-9;
                double waist_z = 600e-9;
                FocusMode mode = FocusMode::EXCITATION;
                std::string input_id = "__coordinates__";
                std::string output_id = "__flux__";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                Coordinate c{0.0,0.0,0.0,0.0};
                TimedValue flux{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Gauss> focus_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> output_ptr;

        };
    }
}
