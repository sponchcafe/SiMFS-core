#ifndef SIM_FOCUS_ALPHA_GAUSS_H
#define SIM_FOCUS_ALPHA_GAUSS_H

#include "focus/base.hpp"

namespace sim{
    namespace focus{

    //-------------------------------------------------------------------//
        class Alpha : public Focus{

            public:

                //-----------------------------------------------------------//
                Alpha();

                //-----------------------------------------------------------//
                Alpha(double w_xy, double w_z);

                //-----------------------------------------------------------//
                void set_waist_xy (double wxy);
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

                double waist_xy = 200e-9;
                double waist_z = 800e-9;
                std::string const type = "alpha";

        };

    }
}

#endif
