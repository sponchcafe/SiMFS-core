#ifndef SIMFS_COMPONENT_MOCK_H
#define SIMFS_COMPONENT_MOCK_H

#include "component/component.hpp"
#include <vector>

//--------------------------------------------------------------------------//
// Fixtures: 
//--------------------------------------------------------------------------//
// Dummy components to test the flow of data across buffers
//
// Producer ---P1---> Transfer ---P2---> Consumer
//--------------------------------------------------------------------------//

using namespace sim::comp;
using namespace sim::io;

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
class ProducerComponent : public Component {

    public:

        //------------------------------------------------------------------//
        ProducerComponent() {}
        void set_json(json j) override {}
        json get_json() override { return json{}; }
        void init() override {
            output = std::make_unique<BufferOutput<int>>(output_id);
        }

        //------------------------------------------------------------------//
        void run() override {
            for (auto &it: data){
                output->put(it);
            }
        }

        //------------------------------------------------------------------//
        void set_output(std::string id){
            output_id = id;
        }

        //------------------------------------------------------------------//
        void set_data(std::vector<int> &d){
            data = d;
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<BufferOutput<int>> output;
        std::vector<int> data{};
        std::string output_id;

};

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
class TransformComponent : public Component {

    public:

        //------------------------------------------------------------------//
        TransformComponent () { }
        void set_json(json j) override {}
        json get_json() override { return json{}; }
        void init() override {
            output = std::make_unique<BufferOutput<int>>(output_id);
            input = std::make_unique<BufferInput<int>>(input_id);
        }

        //------------------------------------------------------------------//
        void run() override {
            int current;
            while(input->get(current)){
                output->put(current);
            }
        }

        //------------------------------------------------------------------//
        void set_output(std::string id){
            output_id = id;
        }

        //------------------------------------------------------------------//
        void set_input(std::string id){
            input_id = id;
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<BufferOutput<int>> output;
        std::unique_ptr<BufferInput<int>> input;
        std::string output_id;
        std::string input_id;

};


//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
class ConsumerComponent : public Component {

    public:

        //------------------------------------------------------------------//
        ConsumerComponent() {}
        void set_json(json j) override {}
        json get_json() override { return json{}; }
        void init() override {
            input = std::make_unique<BufferInput<int>>(input_id);
        }

        //------------------------------------------------------------------//
        void run() override {
            int current;
            while(input->get(current)){
                data.push_back(current);
            }
        }

        //------------------------------------------------------------------//
        void set_input(std::string id){
            input_id = id;
        }
        

        //------------------------------------------------------------------//
        std::vector<int> &get_data(){
            return data;
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<BufferInput<int>> input;
        std::vector<int> data{};
        std::string input_id;

};

#endif
