#include "focus/gaussian.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Gaussian::Gaussian(){ } 

        //------------------------------------------------------------------//
        void Gaussian::set_waist_x (double wx) {
            waist_x = wx;
        }

        //------------------------------------------------------------------//
        void Gaussian::set_waist_y (double wy) {
            waist_y = wy;
        }

        //------------------------------------------------------------------//
        void Gaussian::set_waist_z (double wz) {
            waist_z = wz;
        }
        //------------------------------------------------------------------//
        
        //------------------------------------------------------------------//
        double Gaussian::evaluate(double x, double y, double z) const {
            return gauss(x, waist_x) * gauss(y, waist_y) * gauss(z, waist_z);
        }

        //------------------------------------------------------------------//
        double Gaussian::gauss(double x, double w) const {
            return exp(-4*pow(x, 2)/(2*pow(w, 2)));
        }

    }
}
