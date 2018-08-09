#include "focus/alpha.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Alpha::Alpha(){ } 

        //------------------------------------------------------------------//
        void Alpha::set_waist_xy (double wxy) {
            waist_xy = wxy;
        }

        //------------------------------------------------------------------//
        void Alpha::set_waist_z (double wz) {
            waist_z = wz;
        }

        //------------------------------------------------------------------//
        double Alpha::evaluate(double x, double y, double z) const {
            double x1 = pow(1/(sqrt(sim::CONST_PI/2) * waist_xy), 2);
            double x2 = pow(z,2)/pow(waist_z,2);
            double x3 = (pow(x, 2)+pow(y, 2));
            double x4 = (exp(-2*x2))/pow(waist_xy, 2);
            return x1 * exp(-2*(x2+(x3*x4)));
        }

    }
}
