#include "actions/value_emission_action.hpp"

namespace sim{
    namespace graph{

        std::string ValueEmissionAction::type = "value_emission";

        //-------------------------------------------------------------------//
        ValueEmissionAction::ValueEmissionAction(
                std::string const name,
                sim::graph::Graph &graph,
                comp::Output<TimedValue> &out) :
            Action(graph, name),
            emission_output(out) {
            } 

        //-------------------------------------------------------------------//
        void ValueEmissionAction::set_value(double val) {
            value = val;
        }
        void ValueEmissionAction::set_trigger_node_id(std::string id) {
            trigger_node_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void ValueEmissionAction::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_value(params.at("value"));
            set_trigger_node_id(params.at("node"));

        }

        //-------------------------------------------------------------------//
        json ValueEmissionAction::get_json() {

            json j;
            j["value"] = value;
            j["node"] = trigger_node_id;
            return j;
            
        }

        //-------------------------------------------------------------------//
        void ValueEmissionAction::init() {
            current.value = value;
        }

        //-------------------------------------------------------------------//
        void ValueEmissionAction::fire() {
            current.time = graph.get_clock();
            emission_output.put(current);
        }

    }
}
