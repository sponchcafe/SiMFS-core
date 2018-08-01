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
        void SetStateIfAction::add_pseudo_edge(std::string from, std::string to){
            auto pseudo_edge = std::pair<std::string, std::string>{from, to};
            pseudo_edge_ids.push_back(pseudo_edge);
        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void SetStateIfAction::init() {

            for (auto &ids: pseudo_edge_ids){
                auto pseudo_edge_ptr = std::pair<Node*, Node*>{};
                pseudo_edge_ptr.first = graph.get_node_ptr(ids.first);
                pseudo_edge_ptr.second = graph.get_node_ptr(ids.second);
                pseudo_edge_ptrs.push_back(pseudo_edge_ptr);
            }

            input_ptr->get(current);
            graph.push_event(Event(this, current)); 

        }

        //-------------------------------------------------------------------//
        void SetStateIfAction::fire() {

            Node * current_node = graph.get_current_ptr();
            for (auto &psedge: pseudo_edge_ptrs){
                if (current_node == psedge.first){
                    graph.set_current(psedge.second);
                    break;
                }
            }

            if (input_ptr->get(current)){
                graph.push_event(Event(this, current)); 
            }

        }

    }
}

