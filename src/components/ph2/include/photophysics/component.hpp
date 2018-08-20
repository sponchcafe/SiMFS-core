#include "component/component.hpp"
#include "graph/main.hpp"
#include "actions/set_rates_action.hpp"
#include "actions/output_time_action.hpp"
#include "actions/set_state_if_action.hpp"
#include <unordered_map>
#include <set>

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
                void set_initial_state_id(std::string s);
                //-----------------------------------------------------------//
               
                std::set<std::string> get_timetag_output_ids();
                std::set<std::string> get_flux_input_ids();
                std::set<std::string> get_transition_input_ids();
                std::set<std::string> get_transition_output_ids();

            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                unsigned seed = 0;
                json jablonsky = {
                    {"exi",         {{"from", "S0"}, {"to", "S1"}, {"rate", 1e+7}, {"trigger", {{"input", "__fret__"}, {"output", "__rejected__"}, {"efficiency", 1.0}}}}},
                    {"emi",         {{"from", "S1"}, {"to", "S0"}, {"rate", 1e+9}, {"output", "__emission__"}}},
                    {"isc",         {{"from", "S1"}, {"to", "T1"}, {"rate", 1e+7}}},
                    {"risc",        {{"from", "T1"}, {"to", "S0"}, {"rate", 1e+7}}},
                    {"bleach",      {{"from", "T1"}, {"to", "X0"}, {"rate", 1e+5}}},
                    {"unbleach",    {{"from", "X0"}, {"to", "S0"}, {"trigger", {{"input", "__collisions__"}, {"output", "/dev/null"}, {"efficiency", 1.0}}}}}
                };

                std::string initial_state_id = "S0";
               
                //-Derived-parameters----------------------------------------//
                std::unique_ptr<graph::Graph> graph;

                //-Helpers---------------------------------------------------//
                void add_edges();
                void make_output_actions();
                void make_transition_input_actions();
                void make_flux_input_actions();

                json_filter_t has_output = [] (json const &e) -> bool {
                    return (e.find("output") != e.end());
                };
                json_filter_t has_trigger = [] (json const &e) -> bool {
                    return (e.find("trigger") != e.end());
                };
                json_filter_t has_rate = [] (json const &e) -> bool {
                    return (e.find("rate") != e.end());
                };
                json_filter_t has_constant_rate = [] (json const &e) -> bool {
                    return e["rate"].is_number();
                };
                json_filter_t has_input_rate = [] (json const &e) -> bool {
                    return e["rate"].is_object();
                };

                //-IO-handles------------------------------------------------//
                std::unordered_map<
                    std::string, 
                    std::unique_ptr<io::BufferOutput<realtime_t>>
                        > time_output_map;

                std::unordered_map<
                    std::string, 
                    std::unique_ptr<io::BufferInput<TimedValue>>
                        > flux_input_map;

                std::unordered_map<
                    std::string, 
                    std::unique_ptr<io::BufferInput<realtime_t>>
                        > transition_input_map;

                std::unordered_map<
                    std::string, 
                    std::string
                        > transition_io_map;

                std::unordered_map<
                    std::string, 
                    std::unique_ptr<io::BufferOutput<realtime_t>>
                        > transition_output_map;
        };

    }
}
