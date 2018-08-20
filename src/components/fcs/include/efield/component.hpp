#pragma once

#include "component/component.hpp"
#include "component/types.hpp"
#include "focus/gauss.hpp"
#include <complex>
#include <array>

namespace sim{
    namespace comp{


        //-------------------------------------------------------------------//
        class FCS_EField : public Component{

            public:

                //-----------------------------------------------------------//
                FCS_EField ();

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
                void set_rotation_input_id(std::string id);

                void set_store_id(std::string id);
                void set_grid_x(double x_min, double x_max, size_t n);
                void set_grid_y(double y_min, double y_may, size_t n);
                void set_grid_z(double z_min, double z_max, size_t n);
                void set_theta(double theta_min, double theta_max, size_t n);
                void set_phi(double phi_min, double phi_max, size_t n);
                void set_polarization(bool x; bool y);
                void set_interpolation(std::string type);

                void set_wavelength(double w);
                void set_power(double p);
                //-----------------------------------------------------------//
                 
                
            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double wavelength = 488e-9;                             // m
                double power = 10e-6;                                   // W
                std::string coordinate_input_id = "__coordinates__";
                std::string rotation_input_id = "__rotation__";
                std::string flux_output_id = "__flux__";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                Coordinate c{0.0,0.0,0.0,0.0};
                Coordinate r{0.0,0.0,0.0,0.0}; // Rotation as (x,y,z,t) normalized vector
                TimedValue flux{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Gauss> focus_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> coordinate_input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> flux_output_ptr;

        };
    }
}
