#include "mixer/component.hpp"
#include <algorithm> // sort

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Mixer::Mixer(){ }

        //-------------------------------------------------------------------//
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

            set_photon_output_id(params.at("photon_output"));
            set_photon_input_ids(params.at("photon_inputs"));

        }

        //-------------------------------------------------------------------//
        json Mixer::get_json(){

            json j;

            j["photon_output"] = photon_output_id;
            j["photon_inputs"] = photon_input_ids;

            return j;

        }


        //-------------------------------------------------------------------//
        void Mixer::init() {
        }

        //-------------------------------------------------------------------//
        void Mixer::run(){

            sort_inputs();

            auto first = photon_input_ptrs.begin();
            auto second = first+1;

            while(photon_input_ptrs.size() > 1){
                while (first->get()->peek() <= second->get()->peek()){
                    if(!first->get()->get(current)){
                        std::swap(
                            *photon_input_ptrs.begin(), 
                            *(photon_input_ptrs.end()-1)
                        );
                        photon_input_ptrs.pop_back();
                        break;
                    }
                    photon_output_ptr->put(current);
                }

                sort_inputs();
            }
            while(first->get()->get(current)){
                photon_output_ptr->put(current);
            }

        }

        //-------------------------------------------------------------------//
        void Mixer::sort_inputs(){
            std::sort(
                photon_input_ptrs.begin(),
                photon_input_ptrs.end(),
                [](
                    std::unique_ptr<Input<realtime_t>> &lhs, 
                    std::unique_ptr<Input<realtime_t>> &rhs
                  ) {
                    return *lhs < *rhs;
                }
            );
        }


    }
}
