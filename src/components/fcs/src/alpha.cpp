#include "alpha/component.hpp"
#include "definitions/constants.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        FCS_Alpha::FCS_Alpha() {
            focus_ptr = std::make_unique<focus::Alpha>();
        }

        //-------------------------------------------------------------------//
        void FCS_Alpha::set_output_id(std::string id){
            output_id = id;
        }
        void FCS_Alpha::set_input_id(std::string id){
            input_id = id;
        }
        void FCS_Alpha::set_wavelength(double w){
            wavelength = w;
        }
        void FCS_Alpha::set_power(double p){
            power = p;
        }
        void FCS_Alpha::set_waists(double w_xy, double w_z){
            waist_xy = w_xy;
            waist_z = w_z;
        }
        void FCS_Alpha::set_mode(FocusMode m){
            mode = m;
        }       
        //-------------------------------------------------------------------//

        //-------------------------------------------------------------------//
        void FCS_Alpha::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            if (mode == FocusMode::EXCITATION){
                set_wavelength(params.at("wavelength"));
                set_power(params.at("power"));
            }
            set_input_id(params.at("input"));
            set_output_id(params.at("output"));
            set_waists(
                    params.at("waist_xy"),
                    params.at("waist_z")
                    );
                
        }

        //-------------------------------------------------------------------//
        json FCS_Alpha::get_json() {
            json j;

            if (mode == FocusMode::EXCITATION){
                j["wavelength"] = wavelength;
                j["power"] = power;
            }
            j["input"] = input_id;
            j["output"] = output_id;
            j["waist_xy"] = waist_xy;
            j["waist_z"] = waist_z;

            return j;
        }

        //-------------------------------------------------------------------//
        void FCS_Alpha::init() {
            input_ptr = std::make_unique<io::BufferInput<Coordinate>>(input_id);
            output_ptr = std::make_unique<io::BufferOutput<TimedValue>>(output_id);
        }

        //-------------------------------------------------------------------//
        void FCS_Alpha::run() {

            if (mode == FocusMode::EXCITATION) {
                focus_ptr->set_prefactor(focus_ptr->get_flux_prefactor(power, wavelength));
            } else if (mode == FocusMode::DETECTION) {
                focus_ptr->set_prefactor(focus_ptr->get_efficiency_prefactor());
            }
            while(input_ptr->get(c)){
                flux.time = c.t;
                flux.value = focus_ptr->evaluate(c.x, c.y, c.z);
                output_ptr->put(flux);
            }
        }


    }

}


