#include "splitter/component.hpp"
#include <cmath>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Splitter::Splitter(){ 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Splitter::set_heartbeat(bool hb){
            heartbeat = hb;
        }
        void Splitter::set_seed(unsigned s){
            seed = s;
        }
        void Splitter::set_photon_input_id(std::string id){
            photon_input_id = id;
        }
        void Splitter::set_efficiency_input_id(std::string id){
            efficiency_input_id = id;
        }
        void Splitter::set_accepted_photon_output_id(std::string id){
            accepted_photon_output_id = id;
        }
        void Splitter::set_rejected_photon_output_id(std::string id){
            rejected_photon_output_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Splitter::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_heartbeat(params.at("heartbeat"));
            set_seed(params.at("seed"));
            set_photon_input_id(params.at("photon_input"));
            set_efficiency_input_id(params.at("efficiency_input"));
            set_accepted_photon_output_id(params.at("accepted_output"));
            set_rejected_photon_output_id(params.at("rejected_output"));

        }

        //-------------------------------------------------------------------//
        json Splitter::get_json(){

            json j;

            j["heartbeat"] = heartbeat;
            j["seed"] = seed;
            j["photon_input"] = photon_input_id;
            j["efficiency_input"] = efficiency_input_id;
            j["accepted_output"] = accepted_photon_output_id;
            j["rejected_output"] = rejected_photon_output_id;

            return j;

        }

        //-------------------------------------------------------------------//
        void Splitter::init() {
            efficiency_input_ptr = std::make_unique<io::BufferInput<TimedValue>> (efficiency_input_id);
            photon_input_ptr = std::make_unique<io::BufferInput<realtime_t>> (photon_input_id);
            accepted_photon_output_ptr = std::make_unique<io::BufferOutput<realtime_t>> (accepted_photon_output_id);
            rejected_photon_output_ptr = std::make_unique<io::BufferOutput<realtime_t>> (rejected_photon_output_id);

        }

        //-------------------------------------------------------------------//
        void Splitter::run(){
            
            while(photon_input_ptr->get(timetag)){

                while(fabs(timetag) >= efficiency_input_ptr->peek().time){
                    if (!efficiency_input_ptr->get(efficiency)) break;
                }

                // heartbeat handling
                if (heartbeat && std::signbit(timetag)) {
                    accepted_photon_output_ptr->put(timetag);
                    rejected_photon_output_ptr->put(timetag);
                    continue;
                }

                if (uni() < efficiency.value){
                    accepted_photon_output_ptr->put(timetag);
                }
                else {
                    rejected_photon_output_ptr->put(timetag);
                }

            }

            // tags are done, empty efficiency
            while(efficiency_input_ptr->get(efficiency));

        }

    }
}
