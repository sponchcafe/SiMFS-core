#include "actions/transfer_action.hpp"

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        std::string TransferAction::type = "transfer";

        //-------------------------------------------------------------------//
        TransferAction::TransferAction(
                        std::string const name,
                        sim::graph::Graph &graph) :
            Action(graph, name) {
            }

        //-------------------------------------------------------------------//
        void TransferAction::set_target_edge_id(std::string id){
            target_edge_id = id;
        }
        void TransferAction::set_source_node_id(std::string id){
            source_node_id = id;
        }
        void TransferAction::set_photon_input_id(std::string id){
            photon_input_id = id;
        }
        void TransferAction::set_rejected_photon_output_id(std::string id){
            rejected_photon_output_id = id;
        }
        //-------------------------------------------------------------------//

        //-------------------------------------------------------------------//
        void TransferAction::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_target_edge_id(params.at("target_edge"));
            set_source_node_id(params.at("source_node"));
            set_photon_input_id(params.at("photon_input"));
            set_rejected_photon_output_id(params.at("rejeceted_photon_output"));

        }


        //-------------------------------------------------------------------//
        json TransferAction::get_json(){
            json j;
            j["target_edge"] = target_edge_id;
            j["source_node"] = source_node_id;
            j["photon_input"] = photon_input_id;
            j["rejeceted_photon_output"] = rejected_photon_output_id;
            return j;
        }
        

        //-------------------------------------------------------------------//
        void TransferAction::init() {
            target_edge_ptr = graph.get_edge_ptr(target_edge_id);
            source_node_ptr = graph.get_node_ptr(source_node_id);
            photon_input_ptr->get(current);
            graph.push_event(Event(this, current)); 
        }

        //-------------------------------------------------------------------//
        void TransferAction::fire() {

            if (graph.get_current_ptr() == source_node_ptr) {
                // excitation accepted
                graph.set_current(target_edge_ptr->get_target_node_ptr());
            }
            else{
                // excitation rejetcted
                rejected_photon_output_ptr->put(current);
            }

            if (photon_input_ptr->get(current)){
                graph.push_event(Event(this, current)); 
            }

        }

    }
}

