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
                void add_pseudo_edge(std::string from, std::string to);
                //-----------------------------------------------------------//
                
                sim::graph::Graph &get_graph();

            private:
                

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<realtime_t>> &input_ptr;

                //-----------------------------------------------------------//
                std::vector<std::pair<std::string, std::string>> pseudo_edge_ids;
                std::vector<std::pair<Node *, Node *>> pseudo_edge_ptrs;

                realtime_t current; 

        };

    }
}
