#include "actions/emission_action.hpp"

namespace sim{
    namespace graph{

        std::string EmissionAction::type = "emission";

        //-------------------------------------------------------------------//
        EmissionAction::EmissionAction(
                std::string const name,
                sim::graph::Graph &graph) :
            Action(graph, name) {
            } 

        //-------------------------------------------------------------------//
        void EmissionAction::set_photon_output_id(std::string id){
            photon_output_id = id;
        }
        void EmissionAction::set_trigger_edge_id(std::string id) {
            trigger_edge_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void EmissionAction::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_photon_output_id(params.at("photon_output"));
            set_trigger_edge_id(params.at("trigger_edge"));

        }

        //-------------------------------------------------------------------//
        json EmissionAction::get_json() {

            json j;
            j["photon_output"] = photon_output_id;
            j["trigger_edge"] = trigger_edge_id;
            j["type"] = type;
            return j;
            
        }

        //-------------------------------------------------------------------//
        void EmissionAction::init() {
        }

        //-------------------------------------------------------------------//
        void EmissionAction::fire() {
            double tag = graph.get_clock();
            photon_output_ptr->put(tag);
        }

    }
}
