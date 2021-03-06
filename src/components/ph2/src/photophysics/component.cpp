#include "photophysics/component.hpp"
#include "io/buffer.hpp"
#include "definitions/constants.hpp" // CONST_NA

#include "actions/set_rates_action.hpp"
#include "actions/output_time_action.hpp"
#include "actions/set_state_if_action.hpp"
#include "actions/heartbeat_action.hpp"

using namespace sim::graph;

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Photophysics::Photophysics() { 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Photophysics::set_json(json j){

            json params = get_json();
            if (j.find("jablonsky") != j.end()) params.erase("jablonsky");
            params.merge_patch(j);

            set_seed(params.at("seed"));
            set_initial_state_id(params.at("initial_state"));
            set_jablonsky(params.at("jablonsky"));

        }

        //-------------------------------------------------------------------//
        json Photophysics::get_json(){

            json j;

            j["seed"] = seed;
            j["initial_state"] = initial_state_id;
            j["jablonsky"] = jablonsky;

            return j;

        }

        //-------------------------------------------------------------------//
        std::set<std::string> Photophysics::get_timetag_output_ids(){
            std::set<std::string> ids;
            for (auto &it: time_output_map) ids.insert(it.first);
            return ids;
        }

        //-------------------------------------------------------------------//
        std::set<std::string> Photophysics::get_transition_input_ids(){
            std::set<std::string> ids;
            for (auto &it: transition_input_map) ids.insert(it.first);
            return ids;
        }

        //-------------------------------------------------------------------//
        std::set<std::string> Photophysics::get_transition_output_ids(){
            std::set<std::string> ids;
            for (auto &it: transition_output_map) ids.insert(it.first);
            return ids;
        }

        //-------------------------------------------------------------------//
        std::set<std::string> Photophysics::get_flux_input_ids(){
            std::set<std::string> ids;
            for (auto &it: flux_input_map) ids.insert(it.first);
            return ids;
        }



        //-------------------------------------------------------------------//
        void Photophysics::set_seed(unsigned s){ 
            seed = s; 
        }
        void Photophysics::set_jablonsky(json j){
            jablonsky = j;
        }
        void Photophysics::set_initial_state_id(std::string s){
            initial_state_id = s;
        }
        //-------------------------------------------------------------------//
        

        //-------------------------------------------------------------------//
        void Photophysics::init() {

            graph = std::make_unique<sim::graph::Graph>(seed);

            add_edges();
            make_output_actions();
            make_transition_input_actions();
            make_flux_input_actions();

            graph->set_current(graph->get_node_ptr(initial_state_id));

        }

        //-------------------------------------------------------------------//
        void Photophysics::run(){
            for (auto &it: time_output_map){
                it.second.reset(new io::BufferOutput<realtime_t>(it.first));
            }
            for (auto &it: transition_input_map){
                it.second.reset(new io::BufferInput<realtime_t>(it.first));
            }
            for (auto &it: transition_output_map){
                it.second.reset(new io::BufferOutput<realtime_t>(it.first));
            }
            for (auto &it: flux_input_map){
                it.second.reset(new io::BufferInput<TimedValue>(it.first));
            }
            graph->init();
            graph->traverse();
        }
        

        //-------------------------------------------------------------------//
        // Helpers
        //-------------------------------------------------------------------//
        void Photophysics::add_edges(){

            for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){
                
                auto id = it.key();
                json e = it.value();

                double rate = 0;
                if(has_rate(e) && has_constant_rate(e)){
                    rate = e["rate"];
                }

                graph->add_node(e["from"]);
                graph->add_node(e["to"]);
                graph->add_edge(id, e["from"], e["to"], rate); 

            }

        }

        //-------------------------------------------------------------------//
        void Photophysics::make_output_actions(){

            for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){

                auto id = it.key();
                json e = it.value();

                if (!has_output(e)) continue;

                std::string const output_id = e["output"];
                if (time_output_map.find(output_id) == time_output_map.end()){
                    // no output named "output" -> create a new empty entry
                    time_output_map.emplace(output_id, nullptr);
                }

                std::string action_id = id+".output";
                auto ot_action_ptr = std::make_unique<OutputTimeAction>(
                        action_id,
                        *graph, 
                        time_output_map.at(output_id)
                        );


                std::unique_ptr<Action> act_ptr = std::move(ot_action_ptr);
                graph->add_action(act_ptr);
                graph->link_edge_action(id, action_id);

                // heartbeat
                if (e.find("heartbeat") != e.end() && e["heartbeat"] != 0){
                    std::string hb_action_id = id+".heartbeat";
                    auto hb_action_ptr = std::make_unique<HeartbeatAction>(
                            hb_action_id,
                            *graph, 
                            e["heartbeat"],
                            time_output_map.at(output_id)
                            );
                    std::unique_ptr<Action> hb_act_ptr = std::move(hb_action_ptr);
                    graph->add_action(hb_act_ptr);

                }

            }

        }

        //-------------------------------------------------------------------//
        void Photophysics::make_transition_input_actions(){

            // collect all inputs
            for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){

                auto id = it.key();
                json e = it.value();

                if (!(has_trigger(e))) continue;

                json trigger = e["trigger"];
                std::string input_id = trigger["input"];
                std::string output_id = trigger["output"];

                if (transition_input_map.find(input_id) == transition_input_map.end()){
                    // no input named "input" -> create a new empty entry
                    transition_input_map.emplace(input_id, nullptr);
                    transition_io_map.emplace(input_id, output_id);
                }
                if (transition_output_map.find(output_id) == transition_output_map.end()){
                    // no output named "output" -> create a new empty entry
                    transition_output_map.emplace(output_id, nullptr);
                }
            }

            // for each input collect all 
            for (auto &input: transition_input_map){

                std::string input_id = input.first;

                // create 1 action for each input
                auto ssi_action_ptr = std::make_unique<SetStateIfAction>(
                        input_id+".input",
                        *graph, 
                        input.second,
                        transition_output_map[transition_io_map[input_id]]
                        );

                // collect targets
                for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){
                    std::string edge_id = it.key();
                    json e = it.value();
                    if (!has_trigger(e)) continue;
                    json trigger = e["trigger"];
                    if (trigger["input"] == input_id){
                        ssi_action_ptr->add_node_edge_pair(e["from"], edge_id, trigger["efficiency"]);
                    }
                }

                // add action
                std::unique_ptr<Action> act_ptr = std::move(ssi_action_ptr);
                graph->add_action(act_ptr);

            }

        }

        //-------------------------------------------------------------------//
        void Photophysics::make_flux_input_actions(){

            // collect all inputs
            for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){

                auto id = it.key();
                json e = it.value();

                if (!(has_rate(e) && has_input_rate(e))) continue;

                std::string input_id = e["rate"]["input"];

                if (flux_input_map.find(input_id) == flux_input_map.end()){
                    // no output named "output" -> create a new empty entry
                    flux_input_map.emplace(input_id, nullptr);
                }
            }


            // for each input collect all 
            for (auto &input: flux_input_map){

                std::string input_id = input.first;
                std::vector<std::string> target_edge_ids;
                std::vector<double> scalings;

                // collect targets and epsilon
                for (json::iterator it = jablonsky.begin(); it != jablonsky.end(); ++it){
                    std::string edge_id = it.key();
                    json e = it.value();
                    if (has_rate(e) && has_input_rate(e) && e["rate"]["input"] == input_id){
                        target_edge_ids.push_back(edge_id);
                        double epsilon = e["rate"]["epsilon"];
                        double scaling = epsilon * 100.0/1000 * log(10) / CONST_NA;
                        scalings.push_back(scaling);
                    }
                }

                // create 1 action for each input
                auto sr_action_ptr = std::make_unique<SetRatesAction>(
                        input_id+".input",
                        *graph, 
                        input.second
                        );

                // set list of targets and inputs
                sr_action_ptr->set_target_edge_ids(target_edge_ids);
                sr_action_ptr->set_scalings(scalings);

                // add action
                std::unique_ptr<Action> act_ptr = std::move(sr_action_ptr);
                graph->add_action(act_ptr);

            }

        }

    }
}
