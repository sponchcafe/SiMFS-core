#include "actions/excitation_action.hpp"
#include "focus/base.hpp" // constants

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        std::string ExcitationAction::type = "excitation";

        //-------------------------------------------------------------------//
        ExcitationAction::ExcitationAction(
                std::string const name,
                sim::graph::Graph &graph): 
            Action(graph, name){
            }

        //-------------------------------------------------------------------//
        void ExcitationAction::set_target_edge_id(std::string id){
            target_edge_id = id;
        }
        void ExcitationAction::set_flux_input_id(std::string id){
            flux_input_id = id;
        }
        void ExcitationAction::set_absorption_coefficient(double epsilon){
            absorption_coefficient = fabs(epsilon);
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void ExcitationAction::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_target_edge_id(params.at("target_edge"));
            set_flux_input_id(params.at("flux_input"));
            set_absorption_coefficient(params.at("absorption_coefficient"));

        }

        //-------------------------------------------------------------------//
        json ExcitationAction::get_json() {
            
            json j;

            j["target_edge"] = target_edge_id;
            j["flux_input"] = flux_input_id;
            j["absorption_coefficient"] = absorption_coefficient;
            j["type"] = type;

            return j;

        }

        //-------------------------------------------------------------------//
        void ExcitationAction::init() {
            target = graph.get_edge_ptr(target_edge_id);
            scaling = absorption_coefficient * 100.0/1000 * log(10) / CONST_NA;
            flux_input_ptr->get(current);
            fire();
        }

        //-------------------------------------------------------------------//
        void ExcitationAction::fire() {

            target->set_lambda(scaling * current.value);
            graph.push_event(Event(this, flux_input_ptr->peek().time));

            if (!flux_input_ptr->get(current)){
                graph.set_done(true);
                return;
            }

        }

    }
}
