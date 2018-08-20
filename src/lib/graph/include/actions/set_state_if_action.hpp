#pragma once

#include "graph/main.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class SetStateIfAction : public Action{

            //---------------------------------------------------------------//
            typedef struct{
                std::string node;
                std::string edge;
                double efficiency;
            } node_edge_id;
            //---------------------------------------------------------------//
            typedef struct{
                Node* node;
                Edge* edge;
                double efficiency;
            } node_edge_ptr;
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                SetStateIfAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        std::unique_ptr<io::BufferInput<realtime_t>> &input,
                        std::unique_ptr<io::BufferOutput<realtime_t>> &output);

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void add_node_edge_pair(std::string if_node, std::string then_edge, double efficiency);
                //-----------------------------------------------------------//
                
                sim::graph::Graph &get_graph();

            private:
                

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<realtime_t>> &input_ptr;
                std::unique_ptr<io::BufferOutput<realtime_t>> &output_ptr;

                //-----------------------------------------------------------//
                std::vector<node_edge_id> node_edge_ids;
                std::vector<node_edge_ptr> node_edge_ptrs;

                realtime_t current; 
                random::Uniform uni;

        };

    }
}
