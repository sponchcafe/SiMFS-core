#ifndef SIM_FOCUS_GAUSSIAN_H
#define SIM_FOCUS_GAUSSIAN_H

#include "focus/base.hpp"

namespace sim{
    namespace focus{

        class Gaussian : public BaseFocus{

            public:

                Gaussian(double w_x, double w_y, double w_z) 
                    : waist_x(w_x), waist_y(w_y), waist_z(w_z)
                {}

                double evaluate(double x, double y, double z) const {
                    return 
                        gauss(x, waist_x) *
                        gauss(y, waist_y) * 
                        gauss(z, waist_z);
                }

            private:

                double gauss(double x, double w) const {
                    return exp(-4*pow(x, 2)/(2*pow(w, 2)));
                }

                double waist_x;
                double waist_y;
                double waist_z;

        };

    }
}

#endif
