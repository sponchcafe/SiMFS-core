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
        double Alpha::get_flux_prefactor (double power, double wavelength) {
            double normalization = 1.0/(sim::CONST_PI/2 * pow(waist_xy,2));
            double quantization = power * wavelength / (CONST_C * CONST_H);
            return normalization * quantization;
        }

        //------------------------------------------------------------------//
        double Alpha::get_efficiency_prefactor () {
            return 1.0;
        }
        //------------------------------------------------------------------//


        //------------------------------------------------------------------//
        double Alpha::evaluate(double x, double y, double z) const {
            double x1 = pow(z,2)/pow(waist_z,2);
            double x2 = (pow(x, 2)+pow(y, 2));
            double x3 = (exp(-2*x1))/pow(waist_xy, 2);
            return prefactor * exp(-2*(x1+(x2*x3)));
        }

    }
}
