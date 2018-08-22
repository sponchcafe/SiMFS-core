#include "conformation/component.hpp"
#include "actions/terminate_action.hpp"
#include "actions/output_timed_value_action.hpp"

using namespace sim::graph;

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Conformation::Conformation() { 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Conformation::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_seed(params.at("seed"));
            set_states(params.at("states"));
            set_initial_state_id(params.at("start_at"));
            set_values(params.at("values"));
            set_experiment_time(params.at("experiment_time"));
            set_value_output_id(params.at("output"));

        }

        //-------------------------------------------------------------------//
        json Conformation::get_json(){

            json j;

            j["seed"] = seed;
            j["experiment_time"] = experiment_time;
            j["start_at"] = initial_state_id;
            j["states"] = states;
            j["values"] = values;
            j["output"] = value_output_id;

            return j;

        }

        //-------------------------------------------------------------------//
        void Conformation::set_seed(unsigned s){ 
            seed = s; 
        }
        void Conformation::set_experiment_time(realtime_t t){ 
            experiment_time = fabs(t);
        }
        void Conformation::set_states(json s){
            states = s;
        }
        void Conformation::set_values(json v){
            values = v;
        }
        void Conformation::set_value_output_id(std::string id){
            value_output_id = id;
        }
        void Conformation::set_initial_state_id(std::string id){
            initial_state_id = id;
        }
        //-------------------------------------------------------------------//
        

        //-------------------------------------------------------------------//
        void Conformation::init() {

            // create new graph
            graph = std::make_unique<sim::graph::Graph>(seed);

            // add end action
            std::unique_ptr<Action> terminate_action_ptr = 
                std::make_unique<TerminateAction>("terminate", *graph);
            graph->add_action(terminate_action_ptr);

            // add graph
            for (json::iterator it=states.begin(); it!=states.end(); ++it){
                add_edge(it.key(), it.value());
            }

            // Initialize output
            value_output_ptr = std::make_unique<io::BufferOutput<TimedValue>> (value_output_id);

            // Initialize actions here
            for (json::iterator it=values.begin(); it!=values.end(); ++it){

                // get node name (used as trigger _and_ action name)
                std::string node_name = it.key();

                std::unique_ptr<OutputTimedValueAction> otval_act_uptr =
                    std::make_unique<OutputTimedValueAction>(node_name, *graph, value_output_ptr);
                
                otval_act_uptr->set_value(it.value());
                std::unique_ptr<Action> act_uptr = std::move(otval_act_uptr);

                graph->add_action(act_uptr);
                graph->link_node_action(node_name, node_name); // trigger node name, action name
                
            }

            graph->init();

            // Set initial node
            graph->set_current(graph->get_node_ptr(initial_state_id));

            // bootstrap termination
            graph->push_event(Event(graph->get_action_ptr("terminate"), experiment_time));
        }

        //-------------------------------------------------------------------//
        void Conformation::run(){
            graph->traverse();
        }

        //-------------------------------------------------------------------//
        void Conformation::add_edge(std::string id, json j){
            graph->add_node(j["from"]);
            graph->add_node(j["to"]);
            graph->add_edge(id, j["from"], j["to"], j["rate"]);
        }

    }
}
