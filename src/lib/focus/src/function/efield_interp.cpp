#include "function/efield_interp.hpp"
#include <chrono>
#include <thread>

namespace sim{
    namespace focus{

        //-----------------------------------------------------------//
        FieldInterpolator::FieldInterpolator(){
        }

        //-----------------------------------------------------------//
        void FieldInterpolator::set_field_file(std::string fname){

            field_file_name = fname;
            if(!load_field_file(field_file_name, spec, field)) exit(1);

            x_step = get_grid_step(spec.x);
            y_step = get_grid_step(spec.y);
            z_step = get_grid_step(spec.z);

            x_inc = spec.y.n * spec.z.n;
            y_inc =            spec.z.n;
            z_inc = 1;

        }

        //-----------------------------------------------------------//
        void FieldInterpolator::set_interpolation(Interpolation type){
            interpolation = type;
        }
        void FieldInterpolator::set_rotation(double x, double y, double z){
            rot_x = x;
            rot_y = y;
            rot_z = z;
        }
        //-----------------------------------------------------------//

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
        size_t FieldInterpolator::largest_lower_index(double x, double y, double z) const{
            
            int ix = (int) ((x-spec.x.min) / x_step);
            int iy = (int) ((y-spec.y.min) / y_step);
            int iz = (int) ((z-spec.z.min) / z_step);

            // lower out of bounds
            ix = (ix < 0) ? 0 : ix;
            iy = (iy < 0) ? 0 : iy;
            iz = (iz < 0) ? 0 : iz;

            // upper out of bounds
            ix = (ix > spec.x.n) ? spec.x.n : ix;
            iy = (iy > spec.y.n) ? spec.y.n : iy;
            iz = (iz > spec.z.n) ? spec.z.n : iz;

            size_t i = ix * x_inc + iy * y_inc + iz * z_inc;

            return i;

        }

        //-----------------------------------------------------------//
        EFieldComponents FieldInterpolator::interp_nearest(double x, double y, double z) const {

            size_t index = largest_lower_index(
                    x + (x_step/2),
                    y + (y_step/2),
                    z + (z_step/2)
                    );

            return field[index];

        }

        //-----------------------------------------------------------//
        EFieldComponents FieldInterpolator::interp_linear(double x, double y, double z) const {

            double xd = fmod(x-spec.x.min, x_step);
            double yd = fmod(y-spec.y.min, y_step);
            double zd = fmod(z-spec.z.min, z_step);

            size_t i000 = largest_lower_index(x, y, z);
            size_t i100 = i000+(1*x_inc);
            size_t i010 = i000+(1*y_inc);
            size_t i110 = i100+(1*y_inc);
            size_t i001 = i000+(1*z_inc);
            size_t i101 = i001+(1*x_inc);
            size_t i011 = i001+(1*y_inc);
            size_t i111 = i110+(1*z_inc);

            EFieldComponents c000 = field[i000]; 
            EFieldComponents c100 = field[i100]; 
            EFieldComponents c010 = field[i010]; 
            EFieldComponents c110 = field[i110]; 
            EFieldComponents c001 = field[i001]; 
            EFieldComponents c101 = field[i101]; 
            EFieldComponents c011 = field[i011]; 
            EFieldComponents c111 = field[i111]; 

            EFieldComponents c00 = c000*(1-xd) + c100*xd;
            EFieldComponents c01 = c001*(1-xd) + c101*xd;
            EFieldComponents c10 = c010*(1-xd) + c110*xd;
            EFieldComponents c11 = c011*(1-xd) + c111*xd;

            EFieldComponents c0 = c00*(1-yd) + c10*xd;
            EFieldComponents c1 = c01*(1-yd) + c11*xd;

            EFieldComponents c = c0*(1-zd) + c1*zd;

            return c;

        }

        //-----------------------------------------------------------//
        double FieldInterpolator::evaluate(double x, double y, double z) const {

            EFieldComponents e;
            switch(interpolation){
                case Interpolation::NEAREST :{
                    e = interp_nearest(x, y, z);
                    break;
                }
                case Interpolation::LINEAR : {
                    e = interp_linear(x, y, z);
                    break;
                }
                default : {}
            }

            double effective_field = 
                rot_x * std::norm(e.x) +
                rot_y * std::norm(e.y) +
                rot_z * std::norm(e.z);

            return prefactor * effective_field;

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
