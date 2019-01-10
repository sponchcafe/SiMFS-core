#include "function/gbeam.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Gbeam::Gbeam(){ } 

        //------------------------------------------------------------------//
        void Gbeam::set_waist (double w_xy) {
            waist_xy = w_xy;
        }

        void Gbeam::set_lambda (double l) {
            lambda = l;
        }

        //------------------------------------------------------------------//
        void Gbeam::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_waist( params.at("waist_xy") );
            set_lambda( params.at("wavelength") );

        }

        //------------------------------------------------------------------//
        json Gbeam::get_json() {

            json j;

            j["waist_xy"] = waist_xy;
            j["wavelength"] = lambda;

            return j;

        }

        //------------------------------------------------------------------//
        double Gbeam::get_flux_density_prefactor () const {
            return 1.0/(sim::CONST_PI/2 * pow(waist_xy,2));
        }

        //------------------------------------------------------------------//
        double Gbeam::get_efficiency_prefactor () const {
            return 1.0;
        }

        //------------------------------------------------------------------//
        double Gbeam::evaluate(double x, double y, double z) const {

            double zR = sim::CONST_PI * std::pow(waist_xy, 2) / lambda;
            double wz = waist_xy * std::sqrt(1+std::pow(z/zR, 2));
            double r2 = std::pow(x, 2)+std::pow(y, 2);
            double res = std::pow(waist_xy/wz, 2) * std::exp(-2*r2/std::pow(wz, 2));

            return prefactor * res;

        }

    }

}
