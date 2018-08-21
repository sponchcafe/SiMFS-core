#include "fcs/component.hpp"
#include "focus/gauss.hpp"
#include "definitions/constants.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        FCS::FCS(FocusMode m) {
            mode = m;
            focus_ptr = std::make_unique<focus::Gauss>();
        }

        //-------------------------------------------------------------------//
        void FCS::set_output_id(std::string id){
            output_id = id;
        }
        void FCS::set_input_id(std::string id){
            input_id = id;
        }
        void FCS::set_wavelength(double w){
            wavelength = w;
        }
        void FCS::set_power(double p){
            power = p;
        }

        //-------------------------------------------------------------------//
        void FCS::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            // configure focus function
            focus_ptr->set_json(params.at("focus"));

            if (mode == FocusMode::EXCITATION){
                set_wavelength(params.at("wavelength"));
                set_power(params.at("power"));
            }

            set_input_id(params.at("input"));
            set_output_id(params.at("output"));

        }

        //-------------------------------------------------------------------//
        json FCS::get_json() {

            json j;

            if (mode == FocusMode::EXCITATION){
                j["wavelength"] = wavelength;
                j["power"] = power;
            }

            j["input"] = input_id;
            j["output"] = output_id;

            // get back focus config
            j["focus"] = focus_ptr->get_json();

            return j;
        }

        //-------------------------------------------------------------------//
        void FCS::set_focus_ptr(std::unique_ptr<focus::Focus> &f){
            focus_ptr = std::move(f);
        }

        //-------------------------------------------------------------------//
        void FCS::init() {

            input_ptr = std::make_unique<io::BufferInput<Coordinate>>(input_id);
            output_ptr = std::make_unique<io::BufferOutput<TimedValue>>(output_id);

            if (mode == FocusMode::EXCITATION) {
                double quantization = power * wavelength / (CONST_H * CONST_C);
                focus_ptr->set_prefactor(quantization * focus_ptr->get_flux_density_prefactor());
            } else if (mode == FocusMode::DETECTION) {
                focus_ptr->set_prefactor(focus_ptr->get_efficiency_prefactor());
            }

        }

        //-------------------------------------------------------------------//
        void FCS::run() {

            while(input_ptr->get(c)){
                current.time = c.t;
                current.value = focus_ptr->evaluate(c.x, c.y, c.z);
                output_ptr->put(current);
            }

        }


    }

}


