#include "actions/set_state_if_action.hpp"

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        std::string SetStateIfAction::type = "set_state_if";

        //-------------------------------------------------------------------//
        SetStateIfAction::SetStateIfAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::unique_ptr<io::BufferInput<realtime_t>> &input) :
            Action(graph, name),
            input_ptr(input){
            }

        //-------------------------------------------------------------------//
        void SetStateIfAction::add_node_edge_pair(std::string if_node, std::string then_edge){
            auto node_edge_id = std::pair<std::string, std::string>{if_node, then_edge};
            node_edge_ids.push_back(node_edge_id);
        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void SetStateIfAction::init() {

            for (auto &ids: node_edge_ids){
                auto node_edge_ptr = std::pair<Node*, Edge*>{};
                node_edge_ptr.first = graph.get_node_ptr(ids.first);
                node_edge_ptr.second = graph.get_edge_ptr(ids.second);
                node_edge_ptrs.push_back(node_edge_ptr);
            }

            input_ptr->get(current);
            graph.push_event(Event(this, current)); 

        }

        //-------------------------------------------------------------------//
        void SetStateIfAction::fire() {

            Node * current_node = graph.get_current_ptr();
            for (auto &node_edge_ptr: node_edge_ptrs){
                if (current_node == node_edge_ptr.first){
                    node_edge_ptr.second->traverse();
                    graph.set_current(node_edge_ptr.second->get_target_node_ptr());
                    break;
                }
            }

            if (input_ptr->get(current)){
                graph.push_event(Event(this, current)); 
            }

        }

    }
}

