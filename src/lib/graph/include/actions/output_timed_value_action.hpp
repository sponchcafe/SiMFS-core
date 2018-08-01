#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class OutputTimedValueAction : public Action{

            //---------------------------------------------------------------//
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                OutputTimedValueAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        std::unique_ptr<io::BufferOutput<TimedValue>> &output
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void set_value(double val);

            private:

                //-----------------------------------------------------------//
                TimedValue tvalue{0, 0};

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferOutput<TimedValue>> &output_ptr;

        };
    }
}
