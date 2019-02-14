#include "detection/component.hpp"
#include "definitions/constants.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        void Detection::set_max_efficiency(double e){
            max_efficiency = e;
        }

        //-------------------------------------------------------------------//
        void Detection::set_json(json j){

            json params = Focus::get_json();
            params.merge_patch(get_json());

            params.merge_patch(j);

            Focus::set_json(params);

            set_max_efficiency(params.at("max_efficiency"));


        }

        //-------------------------------------------------------------------//
        json Detection::get_json() {

            json j = Focus::get_json();

            j["max_efficiency"] = max_efficiency;

            return j;

        }

        //-------------------------------------------------------------------//
        void Detection::init() {

            Focus::init();

            focus_shape_ptr->set_prefactor( 
                max_efficiency
                * focus_shape_ptr->get_efficiency_prefactor()
            );

        }

        void Detection::run() {
            Focus::run();
        }

    }

}


