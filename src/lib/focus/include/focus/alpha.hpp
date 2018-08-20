#pragma once

#include "focus/base.hpp"

namespace sim{
    namespace focus{

    //-------------------------------------------------------------------//
        class Alpha : public Focus{

            public:

                //-----------------------------------------------------------//
                Alpha();

                //-----------------------------------------------------------//
                void set_waist_xy (double wxy);
                void set_waist_z (double wz);

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_prefactor(double power, double wavelength);
                double get_efficiency_prefactor();

            private:

                double waist_xy = 200e-9;
                double waist_z = 800e-9;

        };

    }
}

