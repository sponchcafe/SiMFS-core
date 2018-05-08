#ifndef SIMFS_COMPONENT_MOCK_H
#define SIMFS_COMPONENT_MOCK_H

#include "component/component.hpp"
#include <vector>

//--------------------------------------------------------------------------//
// Fixtures: 
//--------------------------------------------------------------------------//
// Dummy components to test the flow of data with different IO implementations
//
// Producer ---P1---> Transfer ---P2---> Consumer
// Files (serial)
// Pipes (parallel)
// Vectors (serial)
// Queues (parallel)
//--------------------------------------------------------------------------//

using namespace sim;

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
template <
    template<class InputT> class InputT,
    template<class OutputT> class OutputT
    > class ProducerComponent : public Component <InputT, OutputT> {

    public:

        //------------------------------------------------------------------//
        ProducerComponent<InputT, OutputT>() { }

        //------------------------------------------------------------------//
        void run() override {
            for (auto &it: data){
                output->put(it);
            }
        }

        //------------------------------------------------------------------//
        void set_output(std::string outname){
            output = std::make_unique<OutputT<int>>(outname);
        }

        //------------------------------------------------------------------//
        void set_data(std::vector<int> &d){
            data = d;
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<OutputT<int>> output;
        std::vector<int> data{};

};

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
template <
    template<class InputT> class InputT,
    template<class OutputT> class OutputT
    > class TransformComponent : public Component <InputT, OutputT> {

    public:

        //------------------------------------------------------------------//
        TransformComponent<InputT, OutputT> () : 
            Component<InputT, OutputT>() {
        }

        //------------------------------------------------------------------//
        void run() override {
            int current;
            while(input->get(current)){
                output->put(current);
            }
        }

        //------------------------------------------------------------------//
        void set_output(std::string outname){
            output = std::make_unique<OutputT<int>>(outname);
        }

        //------------------------------------------------------------------//
        void set_input(std::string inname){
            input = std::make_unique<InputT<int>>(inname);
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<OutputT<int>> output;
        std::unique_ptr<InputT<int>> input;

};


//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
template <
    template<class InputT> class InputT,
    template<class OutputT> class OutputT
    > class ConsumerComponent : public Component <InputT, OutputT> {

    public:

        //------------------------------------------------------------------//
        ConsumerComponent<InputT, OutputT>() {}

        //------------------------------------------------------------------//
        void run(){
            int current;
            while(input->get(current)){
                data.push_back(current);
            }
        }

        //------------------------------------------------------------------//
        void set_input(std::string inname){
            input = std::make_unique<InputT<int>>(inname);
        }

        //------------------------------------------------------------------//
        std::vector<int> &get_data(){
            return data;
        }

    private:

        //------------------------------------------------------------------//
        std::unique_ptr<InputT<int>> input;
        std::vector<int> data{};

};

#endif
