#pragma once

#include "component/component.hpp"
#include "component/types.hpp"
#include "focus/gauss.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class FCS_Flux : public Component{

            public:

                //-----------------------------------------------------------//
                FCS_Flux ();

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
                void set_flux_output_id(std::string id);
                void set_coordinate_input_id(std::string id);
                void set_wavelength(double w);
                void set_power(double p);
                void set_waist_xy(double w_xy);
                void set_waist_z(double w_z);
                //-----------------------------------------------------------//
                 
                
            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double wavelength = 488e-9;                             // m
                double power = 10e-6;                                   // W
                double waist_xy = 200e-9;
                double waist_z = 200e-9;
                std::string coordinate_input_id = "__coordinates__";
                std::string flux_output_id = "__flux__";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                Coordinate c{0.0,0.0,0.0,0.0};
                TimedValue flux{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Flux> focus_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> coordinate_input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> flux_output_ptr;

        };
    }
}
