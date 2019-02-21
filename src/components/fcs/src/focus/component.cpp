#include "focus/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Focus::Focus() {
        }

        //-------------------------------------------------------------------//
        void Focus::set_shape_type(focus::ShapeType type){
            shape_type = type;
        }
        void Focus::set_output_id(std::string id){
            output_id = id;
        }
        void Focus::set_input_id(std::string id){
            input_id = id;
        }
        void Focus::set_focus_shape_ptr(std::unique_ptr<focus::FocusShape> &f){
            focus_shape_ptr = std::move(f);
        }

        //-------------------------------------------------------------------//
        void Focus::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_shape_type(params.at("type").get<focus::ShapeType>());
            set_input_id(params.at("input"));
            set_output_id(params.at("output"));

            json shape_params = params["shape"];
            auto shape = focus_shape_factory(shape_type, shape_params);
            set_focus_shape_ptr(shape);

        }

        //-------------------------------------------------------------------//
        json Focus::get_json() {

            json j;

            j["input"] = input_id;
            j["output"] = output_id;

            j["type"] = shape_type;
            // get back focus config
            if (focus_shape_ptr) {
                j["shape"] = focus_shape_ptr->get_json();
            } else{
                j["shape"] = json::object();
            }

            return j;
        }


        //-------------------------------------------------------------------//
        void Focus::init() {

            input_ptr = std::make_unique<io::BufferInput<Coordinate>>(input_id);
            output_ptr = std::make_unique<io::BufferOutput<TimedValue>>(output_id);

            focus_shape_ptr->init();

        }

        //-------------------------------------------------------------------//
        void Focus::run() {

            Coordinate coord{0,0,0,0};
            TimedValue value{0,0};

            while(input_ptr->get(coord)){
                value.time = coord.t;
                value.value = focus_shape_ptr->evaluate(coord.x, coord.y, coord.z);
                output_ptr->put(value);
            }

        }

    }

}


