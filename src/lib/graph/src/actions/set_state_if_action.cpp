#include "actions/set_state_if_action.hpp"

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        std::string SetStateIfAction::type = "set_state_if";

        //-------------------------------------------------------------------//
        SetStateIfAction::SetStateIfAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::unique_ptr<io::BufferInput<realtime_t>> &input,
                std::unique_ptr<io::BufferOutput<realtime_t>> &output) :
            Action(graph, name),
            input_ptr(input),
            output_ptr(output),
            uni(graph.get_new_seed()){
            }

        //-------------------------------------------------------------------//
        void SetStateIfAction::add_node_edge_pair(std::string if_node, std::string then_edge, double efficiency=1.0){
            auto ne_id = node_edge_id{if_node, then_edge, efficiency};
            node_edge_ids.push_back(ne_id);
        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void SetStateIfAction::init() {

            for (auto &ids: node_edge_ids){
                node_edge_ptr ne_ptr{};
                ne_ptr.node= graph.get_node_ptr(ids.node);
                ne_ptr.edge = graph.get_edge_ptr(ids.edge);
                ne_ptr.efficiency = ids.efficiency;
                node_edge_ptrs.push_back(ne_ptr);
            }

            input_ptr->get(current);
            graph.push_event(Event(this, current)); 

        }

        //-------------------------------------------------------------------//
        void SetStateIfAction::fire() {

            Node * current_node = graph.get_current_ptr();
            bool accepted = false;
            for (auto &ne_ptr: node_edge_ptrs){
                if (current_node == ne_ptr.node){
                    if (uni() < ne_ptr.efficiency){
                        ne_ptr.edge->traverse();
                        graph.set_current(ne_ptr.edge->get_target_node_ptr());
                        accepted=true;
                        break;
                    }
                }
            }

            if (!accepted){
                output_ptr->put(current);
            }

            if (input_ptr->get(current)){
                graph.push_event(Event(this, current)); 
            }

        }

    }
}

