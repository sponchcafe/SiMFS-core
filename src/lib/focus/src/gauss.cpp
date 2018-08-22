#include "focus/gauss.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Gauss::Gauss(){ } 

        //------------------------------------------------------------------//
        void Gauss::set_waists (double w_x, double w_y, double w_z) {
            waist_x = w_x;
            waist_y = w_y;
            waist_z = w_z;
        }

        //------------------------------------------------------------------//
        void Gauss::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_waists(
                    params.at("waist_x"), 
                    params.at("waist_y"),
                    params.at("waist_z")
                    );

        }

        //------------------------------------------------------------------//
        json Gauss::get_json() {

            json j;

            j["waist_x"] = waist_x;
            j["waist_y"] = waist_y;
            j["waist_z"] = waist_z;

            return j;

        }

        //------------------------------------------------------------------//
        double Gauss::get_flux_density_prefactor() const {
            return 1.0/(CONST_PI/2*waist_x*waist_y);
        }
        
        //------------------------------------------------------------------//
        double Gauss::get_efficiency_prefactor() const {
            return 1.0;
        }

        //------------------------------------------------------------------//
        double Gauss::evaluate(double x, double y, double z) const {
            return prefactor * gauss(x, waist_x) * gauss(y, waist_y) * gauss(z, waist_z);
        }

        //------------------------------------------------------------------//
        double Gauss::gauss(double x, double w) const {
            return exp(-4*pow(x, 2)/(2*pow(w, 2)));
        }






    }
}