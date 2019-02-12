#include "shape/XYGaussZExp.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        XYGaussZExp::XYGaussZExp(){ } 

        //------------------------------------------------------------------//
        void XYGaussZExp::set_waists (double w_xy, double w_z) {
            waist_xy = w_xy;
            waist_z  = w_z;
        }

        //------------------------------------------------------------------//
        void XYGaussZExp::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_waists(
                    params.at("waist_xy"),
                    params.at("waist_z")
                    );

        }

        //------------------------------------------------------------------//
        json XYGaussZExp::get_json() {

            json j;

            j["waist_xy"] = waist_xy;
            j["waist_z"] = waist_z;

            return j;

        }

        //------------------------------------------------------------------//
        double XYGaussZExp::get_flux_density_prefactor () const {
            return 1.0/(sim::CONST_PI/2 * pow(waist_xy,2));
        }

        //------------------------------------------------------------------//
        double XYGaussZExp::get_efficiency_prefactor () const {
            return 1.0;
        }

        //------------------------------------------------------------------//
        double XYGaussZExp::evaluate(double x, double y, double z) const {
            double x1 = pow(z,2)/pow(waist_z,2);
            double x2 = (pow(x, 2)+pow(y, 2));
            double x3 = (exp(-2*x1))/pow(waist_xy, 2);
            return prefactor * exp(-2*(x1+(x2*x3)));
        }

    }
}
