#include "gauss/component.hpp"
#include "constants/constants.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        FCS_Gauss::FCS_Gauss() {
            focus_ptr = std::make_unique<focus::Gauss>();
        }

        //-------------------------------------------------------------------//
        void FCS_Gauss::set_flux_output_id(std::string id){
            flux_output_id = id;
        }
        void FCS_Gauss::set_coordinate_input_id(std::string id){
            coordinate_input_id = id;
        }
        void FCS_Gauss::set_wavelength(double w){
            wavelength = w;
        }
        void FCS_Gauss::set_power(double p){
            power = p;
        }
        void FCS_Gauss::set_waists(double w_x, double w_y, double w_z){
            waist_x = w_x;
            waist_y = w_y;
            waist_z = w_z;
        }
        void FCS_Gauss::set_normalize(bool n){
            normalize = n;
        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void FCS_Gauss::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_wavelength(params.at("wavelength"));
            set_power(params.at("power"));
            set_coordinate_input_id(params.at("input"));
            set_flux_output_id(params.at("output"));
            set_waists(
                    params.at("waist_x"),
                    params.at("waist_y"),
                    params.at("waist_z")
                    );
                
        }

        //-------------------------------------------------------------------//
        json FCS_Gauss::get_json() {
            json j;

            j["wavelength"] = wavelength;
            j["power"] = power;
            j["input"] = coordinate_input_id;
            j["output"] = flux_output_id;
            j["waist_x"] = waist_x;
            j["waist_y"] = waist_y;
            j["waist_z"] = waist_z;

            return j;
        }

        //-------------------------------------------------------------------//
        void FCS_Gauss::init() {
            coordinate_input_ptr = 
                std::make_unique<io::BufferInput<Coordinate>>(coordinate_input_id);
            flux_output_ptr = 
                std::make_unique<io::BufferOutput<TimedValue>>(flux_output_id);
        }

        //-------------------------------------------------------------------//
        void FCS_Gauss::run() {
            double const_scaling = power * wavelength / (CONST_C * CONST_H);
            if (normalize) const_scaling = focus_ptr->get_max();
            while(coordinate_input_ptr->get(c)){
                flux.time = c.t;
                flux.value = focus_ptr->evaluate(c.x, c.y, c.z)*const_scaling;
                flux_output_ptr->put(flux);
            }
        }


    }

}


