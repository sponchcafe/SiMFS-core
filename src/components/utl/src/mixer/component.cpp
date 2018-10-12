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
        void Mixer::set_photon_input_ids(std::vector<std::string> ids){
            photon_input_ids = ids;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Mixer::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_heartbeat(params.at("heartbeat"));
            set_photon_output_id(params.at("output"));
            set_photon_input_ids(params.at("inputs"));

        }

        //-------------------------------------------------------------------//
        json Mixer::get_json(){

            json j;

            j["heartbeat"] = heartbeat;
            j["output"] = photon_output_id;
            j["inputs"] = photon_input_ids;

            return j;

        }


        //-------------------------------------------------------------------//
        void Mixer::init() {
            photon_output_ptr = std::make_unique<io::BufferOutput<realtime_t>>(photon_output_id);
            photon_input_ptrs.clear();
            for (auto id: photon_input_ids){
                auto input = std::make_unique<io::BufferInput<realtime_t>>(id);
                photon_input_ptrs.push_back(std::move(input));
            }
        }

        //-------------------------------------------------------------------//
        void Mixer::run(){

            sort_inputs();

            auto first = photon_input_ptrs.begin();
            auto second = first+1;

            while(photon_input_ptrs.size() > 1){
                while (fabs(first->get()->peek()) <= fabs(second->get()->peek())){
                    if(!first->get()->get(current)){
                        std::swap(
                                *photon_input_ptrs.begin(),
                                *(photon_input_ptrs.end()-1)
                                );
                        photon_input_ptrs.pop_back();
                        break;
                    }
                    // heartbeat handling
                    if (!heartbeat && std::signbit(current)) continue;
                    photon_output_ptr->put(current);
                }
                sort_inputs();
            }
            while(first->get()->get(current)){
                // heartbeat handling
                if (!heartbeat && std::signbit(current)) continue;
                photon_output_ptr->put(current);
            }

        }

        //-------------------------------------------------------------------//
        void Mixer::sort_inputs(){
            std::sort(
                photon_input_ptrs.begin(),
                photon_input_ptrs.end(),
                [](
                    std::unique_ptr<io::BufferInput<realtime_t>> &lhs, 
                    std::unique_ptr<io::BufferInput<realtime_t>> &rhs
                  ) {
                    return fabs(lhs->peek()) < fabs(rhs->peek()); // fabs for heartbeat
                }
            );
        }


    }
}
