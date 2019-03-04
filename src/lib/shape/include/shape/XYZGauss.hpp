#pragma once

#include "shape/base.hpp"

namespace sim{
    namespace focus{

        //-------------------------------------------------------------------//
        class XYZGauss : public FocusShape{

            public:

                //-----------------------------------------------------------//
                XYZGauss();

                //-----------------------------------------------------------//
                void set_waists (double w_x, double w_y, double w_z);

                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_density_prefactor() const override;
                double get_efficiency_prefactor() const override;

            private:

                //-----------------------------------------------------------//
                double gauss(double x, double w) const;

                //-----------------------------------------------------------//
                double waist_x = 249e-9;
                double waist_y = 249e-9;
                double waist_z = 637e-9;

        };

    }
}
