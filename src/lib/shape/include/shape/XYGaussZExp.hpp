#pragma once

#include "shape/base.hpp"

namespace sim{
    namespace focus{

    //-------------------------------------------------------------------//
        class XYGaussZExp: public FocusShape{

            public:

                //-----------------------------------------------------------//
                XYGaussZExp();

                //-----------------------------------------------------------//
                void set_waists (double w_xy, double w_z);

                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_density_prefactor() const override;
                double get_efficiency_prefactor() const override;

            private:

                double waist_xy = 249e-9;
                double waist_z = 637e-9;

        };

    }
}

