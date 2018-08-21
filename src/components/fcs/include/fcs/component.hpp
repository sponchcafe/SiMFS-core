#pragma once

#include "component/component.hpp"
#include "focus/base.hpp"

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
                double power = 1e-10;
                double wavelength = 488e-9;
                FocusMode mode = FocusMode::EXCITATION;
                std::string input_id = "__coordinates__";
                std::string output_id = "__focus__";
                //-----------------------------------------------------------//
                
                /*
                 * {
                 *      "input": "__coordinates__",
                 *      "output": "__focus__",
                 *      "power": 1e-6 
                 *      "wavelength": 488e-9,
                 *      "pulse": {
                 *          "time": [],
                 *          "power": []
                 *      },
                 *      "focus_specs": 
                 *      {
                 *          "waist_x": 200e-6
                 *          "waist_y": 200e-6
                 *          "waist_z": 200e-6
                 *      } 
                 *      {
                 *          "waist_xy": 200e-6
                 *          "waist_z": 200e-6
                 *      } 
                 *      {
                 *          "precalculation": {
                 *              "store": "__field__.dat",
                 *              "x_dimension: {
                 *                  "min": 2e-6,
                 *                  "max": 2e-6,
                 *                  "n": 128
                 *              },
                 *              "y_dimension: {
                 *                  "min": 2e-6,
                 *                  "max": 2e-6,
                 *                  "n": 128
                 *              },
                 *              "z_dimension: {
                 *                  "min": 2e-6,
                 *                  "max": 2e-6,
                 *                  "n": 128
                 *              },
                 *              "theta": {
                 *                  "min": 0,
                 *                  "max": 32,
                 *                  "n": 32
                 *              },
                 *              "phi": {
                 *                  "min": 0,
                 *                  "max": 360,
                 *                  "n": 360
                 *               }
                 *          },
                 *
                 *          },
                 *          "store": "__field__.dat",
                 *          "polarization": {
                 *              "x": 1,
                 *              "y": 0
                 *          },
                 *          "roation": {
                 *              "x": 1,
                 *              "y": 0,
                 *              "z": 0
                 *          },
                 *
                 *
                 *          
                 *      }
                 *
                 */

                
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
