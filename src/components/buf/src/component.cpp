#include "buffer/component.hpp"
#include "io/queue_io.hpp"

using namespace sim::queue_io;


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
        }

        //-------------------------------------------------------------------//
        void Buffer::run(){

            std::thread in_thread([this] {input_worker();});
            std::vector<std::thread> out_threads;
            for (auto &it: output_ptrs){
                out_threads.emplace_back([this, &it] {output_worker(it->id, it);});
            }

            in_thread.join();
            for (auto &it: out_threads){
                it.join();
            }

        }


        //-------------------------------------------------------------------//
        void Buffer::input_worker(){

            std::vector<QueueOutput<char>> buffer_inlets;
            for(auto &it: output_ids){
                buffer_inlets.emplace_back(it);
            }

            char current;

            while(input_ptr->get(current)){
                for (auto &it: buffer_inlets){
                    it.put(current);
                }
            }

        }


        //-------------------------------------------------------------------//
        void Buffer::output_worker(std::string id, std::unique_ptr<Output<char>> &out){

            QueueInput<char> buffer_outlet{id};
            char current;
            while (buffer_outlet.get(current)){
                out->put(current);
            }

        }

    }
}
