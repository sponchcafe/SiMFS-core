#pragma once

#include <memory>
#include <vector>
#include "efield/types.hpp"
#include "definitions/constants.hpp"
#include "json/json.hpp"

namespace sim{
    namespace field{

        using json = nlohmann::json;
        
        //-------------------------------------------------------------------//
        class EField {

            public:

                //-----------------------------------------------------------//
                EField();

                //-----------------------------------------------------------//
                void set_lambda(double l);
                void set_pol(bool px, bool py);
                void set_amplitude(double e0);
                void set_theta(double min, double max, size_t n);
                void set_phi(double min, double max, size_t n);
                
                //-----------------------------------------------------------//
                void set_json(json j);
                json get_json();

                //-----------------------------------------------------------//
                EFieldComponents evaluate_angle(double x, double y, double z, size_t i_phi, size_t i_theta) const;
                EFieldComponents evaluate_field(double x, double y, double z) const;

                //-----------------------------------------------------------//
                void init();

            private:

                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                std::vector<double> sin_theta;
                std::vector<double> cos_theta;
                std::vector<double> sin_phi;
                std::vector<double> cos_phi;

                double k = 2*CONST_PI/488e-9;
                std::complex<double> imag_unit{0.0,1.0};

                double e_zero = 1;
                bool pol_x = 1;
                bool pol_y = 0;

                double theta_min = 0;
                double theta_min_deg = 0;
                double theta_max = 64.0/360.0*2*CONST_PI;
                double theta_max_deg= 64.0;
                size_t theta_n = 64;
                double d_theta;

                double phi_min = 0;
                double phi_min_deg = 0;
                double phi_max = 360.0/360.0*2*CONST_PI;
                double phi_max_deg = 360.0;
                size_t phi_n = 360;
                double d_phi;

        };

    }
}
