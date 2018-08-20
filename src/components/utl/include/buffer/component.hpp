#pragma once

#include "component/component.hpp"
#include <thread>
#include <vector>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Buffer : public Component{

            public:

                //-----------------------------------------------------------//
                Buffer ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//


                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_input_id(std::string id);
                void set_output_ids(std::vector<std::string> ids);
                //-----------------------------------------------------------//
               

            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                std::string input_id = "__input__";
                std::vector<std::string> output_ids{"__output1__", "__output2__"};

                std::unique_ptr<io::BufferInput<char>> input_ptr;
                std::vector<std::unique_ptr<io::BufferOutput<char>>> output_ptrs;

        };

    }
}
