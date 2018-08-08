#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class SetStateIfAction : public Action{

            //---------------------------------------------------------------//
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                SetStateIfAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        std::unique_ptr<io::BufferInput<realtime_t>> &input);

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void add_node_edge_pair(std::string if_node, std::string then_edge);
                //-----------------------------------------------------------//
                
                sim::graph::Graph &get_graph();

            private:
                

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<realtime_t>> &input_ptr;

                //-----------------------------------------------------------//
                std::vector<std::pair<std::string, std::string>> node_edge_ids;
                std::vector<std::pair<Node *, Edge *>> node_edge_ptrs;

                realtime_t current; 

        };

    }
}
