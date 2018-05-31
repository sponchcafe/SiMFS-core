#include "component/component.hpp"
#include "component/types.hpp"
#include "graph/main.hpp"
#include "actions/excitation_action.hpp"
#include "actions/emission_action.hpp"
#include "actions/transfer_action.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Photophysics : public Component{

            public:

                //-----------------------------------------------------------//
                Photophysics ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;
                void init() override;
                void run() override;
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_seed(unsigned s);
                void set_jablonsky(json j);
                void set_actions(json a);
                void set_initial_state_id(std::string s);
                //-----------------------------------------------------------//
               
                //-----------------------------------------------------------//
                // get_action
                // Used to get access to the registered actions in order to 
                // connect their specific input and output interfaces.
                //-----------------------------------------------------------//
                template <class ActionT> 
                ActionT &get_action(std::string id){
                    sim::graph::Action *act_ptr = graph.get_action_ptr(id);
                    ActionT &exi_act = dynamic_cast<ActionT&>(*act_ptr);
                    return exi_act;
                }

            private:

                //-Helpers---------------------------------------------------//
                void add_edge(std::string id, json j);
                void configure_action(std::unique_ptr<graph::Action> &act_uptr, json &j);

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                unsigned seed = 0;
                json jablonsky = {
                    {"exi", {{"from", "S0"}, {"to", "S1"}, {"rate", 0}}},
                    {"emi", {{"from", "S1"}, {"to", "S0"}, {"rate", 1e+9}}},
                    {"isc", {{"from", "S1"}, {"to", "T1"}, {"rate", 1e+7}}},
                    {"risc", {{"from", "T1"}, {"to", "S0"}, {"rate", 1e+7}}}
                };
                json actions = {
                    {"__excitation__", {{"type", graph::ExcitationAction::type}}},
                    {"__emission__", {{"type", graph::EmissionAction::type}}}
                };
                std::string initial_state_id = "S0";
               
                //-Derived-parameters----------------------------------------//
                graph::Graph graph;

        };

    }
}
