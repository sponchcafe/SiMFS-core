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
                while (first->get()->peek() <= second->get()->peek()){
                    if(!first->get()->get(current)){
                        std::swap(
                            *photon_input_ptrs.begin(), 
                            *(photon_input_ptrs.end()-1)
                        );
                        photon_input_ptrs.pop_back();
                        std::cerr << "Removing...\n";
                        break;
                    }
                    photon_output_ptr->put(current);
                }
                sort_inputs();
            }
                std::cerr << "last run...\n";
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
                    std::unique_ptr<io::BufferInput<realtime_t>> &lhs, 
                    std::unique_ptr<io::BufferInput<realtime_t>> &rhs
                  ) {
                    return *lhs < *rhs;
                }
            );
        }


    }
}
