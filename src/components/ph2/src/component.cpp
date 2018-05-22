#include "photophysics/component.hpp"
#include "io/file_io.hpp"

using namespace sim::graph;
namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Photophysics::Photophysics() : graph(sim::graph::Graph(0)) { 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Photophysics::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_seed(params.at("seed"));
            set_initial_state_id(params.at("initial_state"));
            set_actions(params.at("actions"));
            set_jablonsky(params.at("jablonsky"));

        }

        //-------------------------------------------------------------------//
        json Photophysics::get_json(){

            json j;

            j["seed"] = seed;
            j["initial_state"] = initial_state_id;
            j["jablonsky"] = jablonsky;
            j["actions"] = actions;

            return j;

        }

        //-------------------------------------------------------------------//
        void Photophysics::set_seed(unsigned s){ 
            seed = s; 
        }
        void Photophysics::set_jablonsky(json j){
            jablonsky = j;
        }
        void Photophysics::set_actions(json a){
            actions = a;
        }
        void Photophysics::set_initial_state_id(std::string s){
            initial_state_id = s;
        }
        //-------------------------------------------------------------------//
        

        //-------------------------------------------------------------------//
        void Photophysics::init() {

            // create new graph
            graph = sim::graph::Graph(seed);

            // add jablonsky
            for (json::iterator it=jablonsky.begin(); it!=jablonsky.end(); ++it){
                add_edge(it.key(), it.value());
            }

            // Set initial node
            graph.set_current(graph.get_node_ptr(initial_state_id));

            // initialize actions here
            for (json::iterator it=actions.begin(); it!=actions.end(); ++it){

                json &j = it.value();
                std::string action_type = j["type"];
                std::string action_name = it.key();

                auto action_uptr = std::unique_ptr<sim::graph::Action>{};

                if (action_type_compare(action_type, ExcitationAction::type)){
                    action_uptr = std::make_unique<ExcitationAction>(action_name, graph);
                    configure_action(action_uptr, j);
                }

                else if (action_type_compare(action_type, EmissionAction::type)){
                    action_uptr = std::make_unique<EmissionAction>(action_name, graph);
                    configure_action(action_uptr, j);
                    graph.link_edge_action(j["trigger_edge"], action_name); // link
                }

                else if (action_type_compare(action_type, TransferAction::type)){
                    action_uptr = std::make_unique<TransferAction>(action_name, graph);
                    configure_action(action_uptr, j);
                }

                else{
                    // ERROR LOG
                    std::cerr << "Type " << action_type << "of action ";
                    std::cerr << action_name << " not recognized.\n";
                    return;
                }

                graph.add_action(action_uptr);

            }

        }

        //-------------------------------------------------------------------//
        void Photophysics::run(){
            graph.init();
            graph.traverse();
        }
        

        //-------------------------------------------------------------------//
        // Helpers
        //-------------------------------------------------------------------//
        void Photophysics::configure_action(std::unique_ptr<Action> &act_uptr,
                json &j){
            json params = act_uptr->get_json();
            params.merge_patch(j);
            j = params;
            act_uptr->set_json(j);
        }

        //-------------------------------------------------------------------//
        void Photophysics::add_edge(std::string id, json j){
            graph.add_node(j["from"]);
            graph.add_node(j["to"]);
            graph.add_edge(id, j["from"], j["to"], j["rate"]);
        }

    }
}
