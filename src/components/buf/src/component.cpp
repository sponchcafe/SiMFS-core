#include "buffer/component.hpp"
#include "io/buffer.hpp"
#include <algorithm>
#include <iterator>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Buffer::Buffer(){ }

        //-------------------------------------------------------------------//
        void Buffer::set_input_id(std::string id){
            input_id = id;
        }
        void Buffer::set_output_ids(std::vector<std::string> ids){
            output_ids = ids;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Buffer::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_input_id(params.at("input"));
            set_output_ids(params.at("outputs"));

        }

        //-------------------------------------------------------------------//
        json Buffer::get_json(){

            json j;

            j["outputs"] = output_ids;
            j["input"] = input_id;

            return j;

        }


        //-------------------------------------------------------------------//
        void Buffer::init() {
            input_ptr = std::make_unique<io::BufferInput<char>> (input_id);
            output_ptrs.clear();
            for (auto &it: output_ids){
                auto output_ptr = std::make_unique<io::BufferOutput<char>> (it);
                output_ptrs.push_back(std::move(output_ptr));
            }
        }

        //-------------------------------------------------------------------//
        void Buffer::run(){

            std::vector<char> chunk{};
            std::vector<std::vector<char>> copies{};
            while(input_ptr->get_chunk(chunk)){
                copies.clear();
                for (int i=0; i<output_ptrs.size(); i++){
                    std::vector<char> copy{};
                    for (auto it=chunk.begin(); it!=chunk.end(); ++it){
                        copy.push_back(*it);
                    }
                    copies.push_back(copy);
                }
                for (int i=0; i<output_ptrs.size(); i++){
                    output_ptrs[i]->put_chunk(copies[i]);
                }
            }

        }




    }
}
