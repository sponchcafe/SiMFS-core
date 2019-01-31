#include "mixer/component.hpp"
#include <algorithm> // sort

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Mixer::Mixer(){ }

        //-------------------------------------------------------------------//
        void Mixer::set_heartbeat(bool hb){
            heartbeat = hb;
        }
        void Mixer::set_photon_output_id(std::string id){
            photon_output_id = id;
        }
        void Mixer::set_routed(bool r){
            routed = r;
        }
        void Mixer::set_photon_input_ids(std::vector<std::string> ids){
            photon_input_ids = ids;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Mixer::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_heartbeat(params.at("heartbeat"));
            set_routed(params.at("routed"));
            set_photon_output_id(params.at("output"));
            set_photon_input_ids(params.at("inputs"));

        }

        //-------------------------------------------------------------------//
        json Mixer::get_json(){

            json j;

            j["heartbeat"] = heartbeat;
            j["routed"] = routed;
            j["output"] = photon_output_id;
            j["inputs"] = photon_input_ids;

            return j;

        }


        //-------------------------------------------------------------------//
        void Mixer::init() {
            if (routed) {
                routed_photon_output_ptr = 
                    std::make_unique<io::BufferOutput<RoutedTime>>(photon_output_id);
            } else {
                single_channel_photon_output_ptr = 
                    std::make_unique<io::BufferOutput<realtime_t>>(photon_output_id);
            }
            photon_input_ptrs.clear();
            for (short unsigned i = 0; i<photon_input_ids.size(); ++i){
                std::string id = photon_input_ids[i];
                RoutedInput input {i, std::make_unique<io::BufferInput<realtime_t>>(id)};
                photon_input_ptrs.push_back(std::move(input));
            }
        }

        //-------------------------------------------------------------------//
        void Mixer::run(){

            sort_inputs();

            auto first = photon_input_ptrs.begin();
            auto second = first+1;

            while(photon_input_ptrs.size() > 1){

                current.route = first->route;

                while (fabs(first->input_ptr->peek()) <= fabs(second->input_ptr->peek())){
                    if(!first->input_ptr->get(current.time)){
                        std::swap(
                                *photon_input_ptrs.begin(),
                                *(photon_input_ptrs.end()-1)
                                );
                        photon_input_ptrs.pop_back();
                        break;
                    }
                    // heartbeat handling
                    if (!heartbeat && std::signbit(current.time)) continue;
                    put_timetag(current);
                }

                sort_inputs();

            }

            while(first->input_ptr->get(current.time)){
                // heartbeat handling
                if (!heartbeat && std::signbit(current.time)) continue;
                put_timetag(current);
            }

        }

        //-------------------------------------------------------------------//
        void Mixer::put_timetag(RoutedTime t){
            if (routed){
                routed_photon_output_ptr->put(current);
            } else {
                single_channel_photon_output_ptr->put(current.time);
            }
        }

        //-------------------------------------------------------------------//
        void Mixer::sort_inputs(){
            std::sort(
                photon_input_ptrs.begin(),
                photon_input_ptrs.end(),
                [](
                    RoutedInput &lhs, 
                    RoutedInput &rhs
                  ) {
                    return fabs(lhs.input_ptr->peek()) < fabs(rhs.input_ptr->peek()); // fabs for heartbeat
                }
            );
        }


    }
}
