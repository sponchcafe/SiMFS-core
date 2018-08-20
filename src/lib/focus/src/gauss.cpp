#include "focus/gauss.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Gauss::Gauss(){ } 

        //------------------------------------------------------------------//
        void Gauss::set_waist_x (double wx) {
            waist_x = wx;
            normalize();
        }

        //------------------------------------------------------------------//
        void Gauss::set_waist_y (double wy) {
            waist_y = wy;
            normalize();
        }

        //------------------------------------------------------------------//
        void Gauss::set_waist_z (double wz) {
            waist_z = wz;
        }
        //------------------------------------------------------------------//
        

        //------------------------------------------------------------------//
        double Gauss::get_max(){
            return evaluate(0, 0, 0);
        }
        
        //------------------------------------------------------------------//
        double Gauss::evaluate(double x, double y, double z) const {
            return norm * gauss(x, waist_x) * gauss(y, waist_y) * gauss(z, waist_z);
        }

        //------------------------------------------------------------------//
        double Gauss::gauss(double x, double w) const {
            return exp(-4*pow(x, 2)/(2*pow(w, 2)));
        }

        //------------------------------------------------------------------//
        void Gauss::normalize () {
            norm = 1.0/(CONST_PI/2*pow(waist_x*waist_y, 2));
        }

    }
}
