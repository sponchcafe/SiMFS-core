#include "focus/alpha.hpp"

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        Alpha::Alpha(){ } 

        //------------------------------------------------------------------//
        Alpha::Alpha(double w_xy, double w_z) : waist_xy(w_xy), waist_z(w_z) {
        }

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

        //------------------------------------------------------------------//
        void Alpha::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);
            set_waist_xy(params.at("waist_xy"));
            set_waist_z(params.at("waist_z"));

        }

        //------------------------------------------------------------------//
        json Alpha::get_json() {

            json j;

            j["waist_xy"] = waist_xy;
            j["waist_z"] = waist_z;
            j["type"] = type;

            return j;

        }
    }
}
