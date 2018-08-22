#include "focus/efield_interp.hpp"

namespace sim{
    namespace focus{


        //-----------------------------------------------------------//
        FieldInterpolator::FieldInterpolator(){
        }

        //-----------------------------------------------------------//
        void FieldInterpolator::set_field_file(std::string fname){
            field_file_name = fname;
            load_field_file(field_file_name, spec, field);
        }
        void FieldInterpolator::set_interpolation(Interpolation type){
            interpolation = type;
        }
        void FieldInterpolator::set_rotation(double x, double y, double z){
            rot_x = x;
            rot_y = y;
            rot_z = z;
        }

        //-----------------------------------------------------------//
        void FieldInterpolator::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_field_file(params.at("field_file"));
            set_interpolation(interpolation_from_str(params.at("interpolation")));
            json rot = params.at("orientation");
            set_rotation(rot.at("x"), rot.at("y"), rot.at("z"));

        }

        //-----------------------------------------------------------//
        json FieldInterpolator::get_json() {

            json j;
            
            j["field_file"] = field_file_name;
            j["interpolation"] = interpolation_to_str(interpolation);
            j["orientation"] = json{
                {"x", rot_x},
                {"y", rot_y},
                {"z", rot_z}
            };

            return j;

        }

        //-----------------------------------------------------------//
        double FieldInterpolator::evaluate(double x, double y, double z) const {



        }

        //-----------------------------------------------------------//
        double FieldInterpolator::get_flux_density_prefactor() const {
            return spec.flux_density_prefactor;
        }

        //-----------------------------------------------------------//
        double FieldInterpolator::get_efficiency_prefactor() const {
            return spec.efficiency_prefactor;
        }

        //-----------------------------------------------------------//
        //-----------------------------------------------------------//

        //-----------------------------------------------------------//
        Interpolation interpolation_from_str(std::string s){
            if (s == "nearest") return Interpolation::NEAREST;
            else if (s == "linear") return Interpolation::LINEAR;
            else{
                std::cerr << "Interpolation " << s << " not understood\n";
                return Interpolation::NEAREST;
            }
        }
        
        //-----------------------------------------------------------//
        std::string interpolation_to_str(Interpolation type){
            switch(type){
                case Interpolation::NEAREST:
                    return "nearest";
                case Interpolation::LINEAR:
                    return "linear";
                default:
                    return "nearest";
            }
        }

    }
}
