#pragma once

#include "shape/base.hpp"

namespace sim{
    namespace focus{

    //-------------------------------------------------------------------//
        class GaussBeam : public FocusShape{

            public:

                //-----------------------------------------------------------//
                GaussBeam();

                //-----------------------------------------------------------//
                void set_waist (double w_xy);
                void set_lambda (double l);

                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_density_prefactor() const override;
                double get_efficiency_prefactor() const override;

            private:

                double waist_xy = 249e-9;
                double lambda = 488e-9;

        };

    }
}

