#pragma once

#include "focus/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Excitation : public Focus{

            public:

                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                void set_json (json j) override;
                json get_json () override;
                void init () override;
                void run () override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_power(double power);
                void set_wavelength(double wavelength);
                //-----------------------------------------------------------//
                 
                
            protected:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double scaling;
                double power = 1e-6;
                double wavelength = 488e-9;
                //-----------------------------------------------------------//
                
        };
    }
}
