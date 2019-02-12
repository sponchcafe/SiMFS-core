#include "shape/gaussBeam.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        GaussBeam::GaussBeam(){ } 

        //------------------------------------------------------------------//
        void GaussBeam::set_waist (double w_xy) {
            waist_xy = w_xy;
        }

        void GaussBeam::set_lambda (double l) {
            lambda = l;
        }

        //------------------------------------------------------------------//
        void GaussBeam::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_waist( params.at("waist_xy") );
            set_lambda( params.at("wavelength") );

        }

        //------------------------------------------------------------------//
        json GaussBeam::get_json() {

            json j;

            j["waist_xy"] = waist_xy;
            j["wavelength"] = lambda;

            return j;

        }

        //------------------------------------------------------------------//
        double GaussBeam::get_flux_density_prefactor () const {
            return 1.0/(sim::CONST_PI/2 * pow(waist_xy,2));
        }

        //------------------------------------------------------------------//
        double GaussBeam::get_efficiency_prefactor () const {
            return 1.0;
        }

        //------------------------------------------------------------------//
        double GaussBeam::evaluate(double x, double y, double z) const {

            double zR = sim::CONST_PI * std::pow(waist_xy, 2) / lambda;
            double wz = waist_xy * std::sqrt(1+std::pow(z/zR, 2));
            double r2 = std::pow(x, 2)+std::pow(y, 2);
            double res = std::pow(waist_xy/wz, 2) * std::exp(-2*r2/std::pow(wz, 2));

            return prefactor * res;

        }

    }

}
