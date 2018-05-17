#include "focus/gaussian.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Gaussian::Gaussian(){ } 

        //------------------------------------------------------------------//
        Gaussian::Gaussian(double w_x, double w_y, double w_z)
            : waist_x(w_x), waist_y(w_y), waist_z(w_z)
        {}

        //------------------------------------------------------------------//
        std::string Gaussian::get_type() const {
            return type;
        }

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

        //------------------------------------------------------------------//
        void Gaussian::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);
            set_waist_x(params.at("waist_x"));
            set_waist_y(params.at("waist_y"));
            set_waist_z(params.at("waist_z"));

        }

        //------------------------------------------------------------------//
        json Gaussian::get_json() {

            json j;

            j["waist_x"] = waist_x;
            j["waist_y"] = waist_y;
            j["waist_z"] = waist_z;
            j["type"] = get_type();

            return j;

        }

    }
}
