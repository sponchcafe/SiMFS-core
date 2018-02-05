#ifndef SIM_FOCUS_ALPHA_GAUSS_H
#define SIM_FOCUS_ALPHA_GAUSS_H

#include "focus/base.hpp"

namespace sim{
    namespace focus{

        class AlphaGauss : public  BaseFocus{

            public:

                AlphaGauss(double w_xy, double w_z) 
                    : waist_xy(w_xy), waist_z(w_z)
                {}

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override{
                    double x1 = pow(1/(sqrt(sim::CONST_PI/2) * waist_xy), 2);
                    double x2 = pow(z,2)/pow(waist_z,2);
                    double x3 = (pow(x, 2)+pow(y, 2));
                    double x4 = (exp(-2*x2))/pow(waist_xy, 2);
                    return x1 * exp(-2*(x2+(x3*x4)));
                }

            private:

                double waist_xy;
                double waist_z;

        };

    }
}

#endif
