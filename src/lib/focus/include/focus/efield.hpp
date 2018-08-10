#pragma once

#include "focus/base.hpp"
#include <vector>
#include <complex>

namespace sim{
    namespace focus{

        //-------------------------------------------------------------------//
        typedef struct {
            std::complex<double> x;
            std::complex<double> y;
            std::complex<double> z;
        } FieldComponents;

        class EField : Focus{

            public:

                //-----------------------------------------------------------//
                EField();

                //-----------------------------------------------------------//
                void set_lambda(double l);
                void set_pol(bool px, bool py);
                void set_dipole(double x, double y, double z);
                void set_amplitude(double e0);
                void set_theta(double min, double max, size_t n);
                void set_phi(double min, double max, size_t n);
                
                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;

                //-----------------------------------------------------------//
                FieldComponents evaluate_components(double x, double y, double z, int i_phi, int i_theta) const;

                //-----------------------------------------------------------//
                FieldComponents integrate_components(double x, double y, double z) const;

                //-----------------------------------------------------------//
                void init();

            private:


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

                double dipole_x = 1.0;
                double dipole_y = 1.0;
                double dipole_z = 1.0;

                double theta_min = 0;
                double theta_max = 32.0/360.0*2*CONST_PI;
                size_t theta_n = 32;
                double d_theta;

                double phi_min = 0;
                double phi_max = 360.0/360.0*2*CONST_PI;
                size_t phi_n = 360;
                double d_phi;

        };

    }
}
