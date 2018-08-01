#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class OutputTimeAction : public Action{

            //---------------------------------------------------------------//
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                OutputTimeAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        std::unique_ptr<io::BufferOutput<realtime_t>> &output
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

            private:

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferOutput<realtime_t>> &output_ptr;

        };
    }
}
