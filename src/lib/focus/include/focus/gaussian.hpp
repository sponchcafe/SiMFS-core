#ifndef SIM_FOCUS_GAUSSIAN_H
#define SIM_FOCUS_GAUSSIAN_H

#include "focus/base.hpp"
#include <cmath>

namespace sim{
    namespace focus{

        class Gaussian : public BaseFocus{

            public:

                Gaussian(){}

                Gaussian(double w_x, double w_y, double w_z) 
                    : waist_x(w_x), waist_y(w_y), waist_z(w_z)
                {
                }

                double evaluate(const sim::SI_Coordinate &c) const {
                    double x = gauss(c.x, waist_x);
                    double y = gauss(c.y, waist_y);
                    double z = gauss(c.z, waist_z);
                    return x*y*z;
                }

            private:

                double gauss(const double &x, const double &w) const {
                    return exp(-4*pow(x, 2)/(2*pow(w, 2)));
                }

                double waist_x;
                double waist_y;
                double waist_z;

        };

    }
}

#endif
