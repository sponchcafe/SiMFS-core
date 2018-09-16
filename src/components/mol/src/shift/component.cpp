#include "shift/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Shifter::Shifter(){ 
        }

        //-------------------------------------------------------------------//
        void Shifter::set_shift(double x, double y, double z) { 
            shift_x = x;
            shift_y = y;
            shift_z = z;
        }
        void Shifter::set_delay(double t){
            delay_t = t;
        }
        void Shifter::set_input_id(std::string id){
            input_id = id;
        }
        void Shifter::set_output_id(std::string id){
            output_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Shifter::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_shift(params.at("shift_x"), params.at("shift_y"), params.at("shift_z"));
            set_delay(params.at("delay"));

        }

        //-------------------------------------------------------------------//
        json Shifter::get_json(){

            json j;

            j["shift_x"] = shift_x;
            j["shift_y"] = shift_y;
            j["shift_z"] = shift_z;
            j["delay"] = delay_t;
            j["input"] = input_id;
            j["output"] = output_id;

            return j;

        }


        //-------------------------------------------------------------------//
        void Shifter::init() {
            
            // IO initialization
            input_ptr = std::make_unique< io::BufferInput<Coordinate> >(input_id);
            output_ptr = std::make_unique< io::BufferOutput<Coordinate> >(output_id);

        }

        //-------------------------------------------------------------------//
        void Shifter::run(){
            Coordinate c;
            while(input_ptr->get(c)){
                c.x += shift_x;
                c.y += shift_y;
                c.z += shift_z;
                c.t += delay_t;
                output_ptr->put(c);
            }

        }

    }
}
