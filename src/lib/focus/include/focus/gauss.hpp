#pragma once

#include "focus/base.hpp"

namespace sim{
    namespace focus{

        //-------------------------------------------------------------------//
        class Gauss : public Focus{

            public:

                //-----------------------------------------------------------//
                Gauss();

                //-----------------------------------------------------------//
                void set_waist_x (double wx);
                void set_waist_y (double wy);
                void set_waist_z (double wz);

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_prefactor(double power, double wavelength);
                double get_efficiency_prefactor();

            private:

                //-----------------------------------------------------------//
                double gauss(double x, double w) const;

                //-----------------------------------------------------------//
                double waist_x = 200e-9;
                double waist_y = 200e-9;
                double waist_z = 800e-9;

        };

    }
}
