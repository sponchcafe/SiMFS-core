#include "detection/component.hpp"
#include "focus/alpha.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Detection::Detection() {
            focus_ptr = std::make_unique<focus::Alpha>();
        }

        //-------------------------------------------------------------------//
        void Detection::set_origin_efficiency(double e){
            origin_efficiency = fabs(e);
        }
        void Detection::set_coordinate_input_id(std::string id){
            coordinate_input_id = id;
        }
        void Detection::set_efficiency_output_id(std::string id){
            efficiency_output_id = id;
        }
        void Detection::set_focus(json params){
            if (params.at("type") == "alpha"){
                focus_ptr = std::make_unique<focus::Alpha>();
                focus_ptr->set_json(params);
            }
            else {
                // ERROR LOG
                std::cerr << "Focus type '" << params.at("type") << "' not recognized.\n";
            }
        }
        //-------------------------------------------------------------------//

        //-------------------------------------------------------------------//
        void Detection::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_origin_efficiency(params.at("origin_efficiency"));
            set_focus(params.at("focus"));
            set_coordinate_input_id(params.at("coordinate_input"));
            set_efficiency_output_id(params.at("efficiency_output"));

        }

        //-------------------------------------------------------------------//
        json Detection::get_json() {

            json j;

            j["origin_efficiency"] = origin_efficiency;
            j["focus"] = focus_ptr->get_json();
            j["coordinate_input"] = coordinate_input_id;
            j["efficiency_output"] = efficiency_output_id;

            return j;

        }

        //-------------------------------------------------------------------//
        void Detection::init(){
            focus_scaling = origin_efficiency / focus_ptr->evaluate(0, 0, 0);
            coordinate_input_ptr = 
                std::make_unique<io::BufferInput<Coordinate>>(coordinate_input_id);
            efficiency_output_ptr = 
                std::make_unique<io::BufferOutput<TimedValue>>(efficiency_output_id);
        }

        //-------------------------------------------------------------------//
        void Detection::run(){
            while(coordinate_input_ptr->get(c)){
                efficiency.time = c.t;
                efficiency.value = focus_ptr->evaluate(c.x, c.y, c.z) * focus_scaling;
                efficiency_output_ptr->put(efficiency);
            }
        }

    }
}
