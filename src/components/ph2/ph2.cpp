#include "shellutils/shellutils.hpp"
#include "graph/main.hpp"
#include "focus/base.hpp" // for constants

class ExcitationAction : public sim::graph::Action{
    
    //-----------------------------------------------------------------------//
    // The excitation action reads a flux value from an input stream,
    // calculates the excitation rate for its target edge according
    // to the specified absorption coefficient and wavelength. It updates
    // the edge lambda and issues a new update event a time increment later
    // until the input stream ends. It then switches the graph's done flag
    // to true.
    //-----------------------------------------------------------------------//
    
    public:

        //-------------------------------------------------------------------//
        ExcitationAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::string const target_edge_name,
                double abs_coef,
                std::string const input_name
                ) :
            Action(graph, name),
            target_edge_name(target_edge_name),
            absorption_coefficient(abs_coef),
            input(sim::io::Input<sim::TimedValue>(input_name)){
            }


        //-------------------------------------------------------------------//
        void init() override {
            target = graph.get_edge_ptr(target_edge_name);
            scaling = absorption_coefficient * 100.0/1000 * log(10) / sim::CONST_NA;
            fire();
        }

        //-------------------------------------------------------------------//
        void fire() override {
            target->set_lambda(scaling*current.value);

            graph.push_event(sim::graph::Event(this, input.peek().time));
            if (!input.get(current)){
               graph.set_done(true);
               return;
            }
        }



    private:

        std::string const target_edge_name;
        double const absorption_coefficient = 0.0;
        double scaling = 0.0;
        sim::graph::Edge *target = nullptr;
        sim::io::Input<sim::TimedValue> input;

        sim::TimedValue current{0.0,0.0};

};

class EmissionAction : public sim::graph::Action{

    public: 
        //-------------------------------------------------------------------//
        EmissionAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::string const output_name
                ) :
            Action(graph, name), output(sim::io::Output<sim::realtime_t>(output_name)){
            }

        //-------------------------------------------------------------------//
        void init() override {
        }
        
        //-------------------------------------------------------------------//
        void fire() override {
            double clock = graph.get_clock();
            output.put(clock);
        }


    private:

        sim::io::Output<sim::realtime_t>output;

};

const std::string helpmessage = 
R"(
Info
Usage: ph2 [options]
)";

json jablonsky_default = {
    {"exi", {{"from", "S0"}, {"to", "S1"}, {"rate", 1e+8}, {"epsilon", 73000}}},
    {"emi", {{"from", "S1"}, {"to", "S0"}, {"rate", 1e+8}}},
    {"isc", {{"from", "S1"}, {"to", "T1"}, {"rate", 1e+8}}},
    {"risc", {{"from", "T1"}, {"to", "S0"}, {"rate", 1e+8}}}
};

json inputs_default =  
{
    {"excitation", {{"target-edge", "exi"}, {"stream", ""}}}
};

json output_default =  
{
    {"emission", {{"target-edge", "emi"}, {"stream", ""}}},
};

int main (int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, 
        {"SIM_PARAMS", "SIM_PH2_PARAMS"}
    };
    jcli::JsonSpec spec{cli};
    spec.enable_scoping();  // --scope

    //----------------------------------------------------------------------//
    unsigned seed = spec.get_option<unsigned>(
            "seed", {"s", "S"}, sim::random::get_new_seed(),
            "Random seed."
            );

    //----------------------------------------------------------------------//
    json jablonsky = spec.get_option<json>(
            "jablonsky", {"y", "Y"}, jablonsky_default,
            "The jablonsky diagram."
            );

    //-----------------------------------------------------------------------//
    std::string start_at = spec.get_option<std::string>(
            "start-at", {"a", "A"}, jablonsky.begin()->at("from"),
            "Initial state."
            );

    //----------------------------------------------------------------------//
    json inputs = spec.get_option<json>(
            "inputs", {"i", "I"}, inputs_default,
            "Inputs."
            );

    //----------------------------------------------------------------------//
    json outputs = spec.get_option<json>(
            "outputs", {"o", "O"}, output_default,
            "Outputs."
            );

    spec.enable_help();
    spec.enable_config();
    spec.enable_log();
    spec.enable_debug();

    // Building the graph

    sim::graph::Graph graph(seed);
    
    //-----------------------------------------------------------------------//
    for (json::iterator it=jablonsky.begin(); it!=jablonsky.end(); ++it){
        json val = it.value();
        graph.add_node(val["from"]);
        graph.add_node(val["to"]);
        graph.add_edge(it.key(), val["from"], val["to"], val["rate"]);
    }

    //-----------------------------------------------------------------------//
    for (json::iterator it=inputs.begin(); it!=inputs.end(); ++it){

        json val = it.value();
        std::string target_edge_name = val["target-edge"];

        double abs_coef = jablonsky[target_edge_name]["epsilon"];
        std::string input_name = val["stream"];

        std::unique_ptr<sim::graph::Action> ex_act_ptr{
            new ExcitationAction(
                    it.key(),
                    graph,
                    target_edge_name,
                    abs_coef,
                    input_name
                    )
        };

        graph.add_action(ex_act_ptr);

    }


    //-----------------------------------------------------------------------//
    for (json::iterator it=outputs.begin(); it!=outputs.end(); ++it){

        json val = it.value();
        std::string output_name = val["stream"];
        std::string target_edge = val["target-edge"];

        std::unique_ptr<sim::graph::Action> emi_act_ptr{
            new EmissionAction(
                    it.key(),
                    graph,
                    output_name
                    )
        };

        graph.add_action(emi_act_ptr);
        graph.link_edge_action(target_edge, it.key());

    }

    //-----------------------------------------------------------------------//
    graph.set_current(graph.get_node_ptr(start_at));
    graph.init();

    graph.traverse();

}
