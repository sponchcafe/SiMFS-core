#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class TerminateAction : public Action{

            //---------------------------------------------------------------//
            // The end action terminates the simulation by setting the graph's
            // done flag.
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                TerminateAction(
                        std::string const name,
                        sim::graph::Graph &graph
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;


            private:


        };
    }
}
