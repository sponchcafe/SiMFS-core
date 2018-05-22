#include "conformation/component.hpp"
#include "actions/end_action.hpp"
#include "actions/value_emission_action.hpp"

using namespace sim::graph;

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Conformation::Conformation() : 
            graph(sim::graph::Graph(0)) { 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Conformation::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_seed(params.at("seed"));
            set_states(params.at("states"));
            set_values(params.at("values"));
            set_experiment_time(params.at("experiment_time"));
            set_value_output_id(params.at("output"));

        }

        //-------------------------------------------------------------------//
        json Conformation::get_json(){

            json j;

            j["seed"] = seed;
            j["experiment_time"] = experiment_time;
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
            graph = sim::graph::Graph(seed);

            // add end action
            std::unique_ptr<Action> end_action_uptr = std::make_unique<EndAction>("end", graph);
            graph.add_action(end_action_uptr);

            // add jablonsky
            for (json::iterator it=states.begin(); it!=states.end(); ++it){
                add_edge(it.key(), it.value());
            }

            // Set initial node
            graph.set_current(graph.get_node_ptr(initial_state_id));

            // initialize actions here
            for (json::iterator it=values.begin(); it!=values.end(); ++it){

                json &j = it.value();
                std::string action_name = it.key();

                std::unique_ptr<Action> act_uptr =
                    std::make_unique<ValueEmissionAction> (
                            action_name,
                            graph,
                            *value_output_ptr
                            );

                act_uptr->set_json(j);
                j = act_uptr->get_json();

                graph.add_action(act_uptr);
                graph.link_node_action(j["node"], action_name);
                
            }

            graph.init();
            graph.push_event(Event(graph.get_action_ptr("end"), experiment_time));
        }

        //-------------------------------------------------------------------//
        void Conformation::run(){
            graph.traverse();
        }

        //-------------------------------------------------------------------//
        void Conformation::add_edge(std::string id, json j){
            graph.add_node(j["from"]);
            graph.add_node(j["to"]);
            graph.add_edge(id, j["from"], j["to"], j["rate"]);
        }

    }
}
