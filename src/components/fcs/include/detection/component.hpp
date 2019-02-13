#pragma once

#include "component/component.hpp"
#include "function/base.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class FCS : public Component{

            public:

                //-----------------------------------------------------------//
                FCS ();
                FCS (FocusMode m);

                //-----------------------------------------------------------//
                void set_json (json j) override;
                json get_json () override;
                void init () override;
                void run () override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_output_id(std::string id);
                void set_input_id(std::string id);
                void set_power(double power);
                void set_wavelength(double wavelength);
                void set_focus_ptr(std::unique_ptr<focus::Focus> &f);
                //-----------------------------------------------------------//
                 
                
            protected:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double scaling;
                double power = 1e-6;
                double wavelength = 488e-9;
                FocusMode mode = FocusMode::EXCITATION;
                std::string input_id = "__coordinates__";
                std::string output_id = "__focus__";
                //-----------------------------------------------------------//
                
                
                //-----------------------------------------------------------//
                Coordinate c{0.0,0.0,0.0,0.0};
                TimedValue current{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Focus> focus_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> output_ptr;

        };
    }
}
