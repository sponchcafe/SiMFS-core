#include "excitation/component.hpp"
#include "definitions/constants.hpp"

namespace sim{
    namespace comp{

        void Excitation::set_wavelength(double w){
            wavelength = w;
        }
        void Excitation::set_power(double p){
            power = p;
        }

        //-------------------------------------------------------------------//
        void Excitation::set_json(json j){


            json params = Focus::get_json();
            params.merge_patch(get_json());

            params.merge_patch(j);

            Focus::set_json(params);

            set_wavelength(params.at("wavelength"));
            set_power(params.at("power"));


        }

        //-------------------------------------------------------------------//
        json Excitation::get_json() {

            json j = Focus::get_json();

            j["wavelength"] = wavelength;
            j["power"] = power;

            return j;

        }

        //-------------------------------------------------------------------//
        void Excitation::init() {

            Focus::init();

            focus_shape_ptr->set_prefactor( 
                power * wavelength / (CONST_H * CONST_C) // quantization
                * focus_shape_ptr->get_flux_density_prefactor()
            );

        }

        void Excitation::run() {
            Focus::run();
        }

    }

}


