#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class HeartbeatAction : public Action{

            //---------------------------------------------------------------//
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                HeartbeatAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        double interval,
                        std::unique_ptr<io::BufferOutput<realtime_t>> &output
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

            private:

                //-----------------------------------------------------------//
                double interval = 1;
                std::unique_ptr<io::BufferOutput<realtime_t>> &output_ptr;

        };
    }
}
