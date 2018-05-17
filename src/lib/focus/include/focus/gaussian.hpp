#ifndef SIM_FOCUS_GAUSSIAN_H
#define SIM_FOCUS_GAUSSIAN_H

#include "focus/base.hpp"

namespace sim{
    namespace focus{

        //-------------------------------------------------------------------//
        class Gaussian : public Focus{

            public:

                //-----------------------------------------------------------//
                Gaussian();

                //-----------------------------------------------------------//
                Gaussian(double w_x, double w_y, double w_z);

                //-----------------------------------------------------------//
                void set_waist_x (double wx);
                void set_waist_y (double wy);
                void set_waist_z (double wz);

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                std::string get_type() const override;

            private:

                //-----------------------------------------------------------//
                double gauss(double x, double w) const;

                //-----------------------------------------------------------//
                double waist_x = 200e-9;
                double waist_y = 200e-9;
                double waist_z = 800e-9;

                std::string const type = "gauss";

        };

    }
}

#endif
