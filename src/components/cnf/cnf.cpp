#include "shellutils/shellutils.hpp"
#include "graph/main.hpp"

class EndAction : public sim::graph::Action{

    public:

        EndAction(
                std::string const name,
                sim::graph::Graph &graph
                ) : Action(graph, name) {}

        void fire() override {
            graph.set_done(true);
        }

};

class FRETAction : public sim::graph::Action{
    
    //-----------------------------------------------------------------------//
    // The FRETAction is bound to a node in the directed rate graph. When fired
    // it emits a timed value containing the current graph time (arrival at the
    // node) and the registered FRET efficiency to its output stream.
    //-----------------------------------------------------------------------//
    
    public:

        //-------------------------------------------------------------------//
        FRETAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::string const target_node_name,
                double const transfer_efficiency,
                sim::io::Output<sim::TimedValue> &output
                ) :
            Action(graph, name),
            target_node_name(target_node_name),
            current(sim::TimedValue{0, transfer_efficiency}),
            output(output)
        {
        }

        //-------------------------------------------------------------------//
        void init() override {
            target = graph.get_node_ptr(target_node_name);
        }

        //-------------------------------------------------------------------//
        void fire() override {
            current.time = graph.get_clock();
            output.put(current);
        }


    private:

        std::string const target_node_name;
        sim::TimedValue current;
        sim::graph::Node *target = nullptr;
        sim::io::Output<sim::TimedValue> &output;

};



const std::string helpmessage = 
R"(
Info
Usage: cnf [options]
)";

json states_default = {
    {"open", {{"from", "FC"}, {"to", "FO"}, {"rate", 1e+2}}},
    {"close", {{"from", "FO"}, {"to", "FC"}, {"rate", 1e+2}}},
};

json efficiencies_default = {
    {"FO", 0.3},
    {"FC", 0.7}
};


int main (int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, {"SIM_PARAMS", "SIM_CNF_PARAMS"} };
    jcli::JsonSpec spec{cli};
    spec.enable_scoping();  // --scope

    //----------------------------------------------------------------------//
    unsigned seed = spec.get_option<unsigned>(
            "seed", {"s", "S"}, sim::random::get_new_seed(),
            "Random seed."
            );

    //----------------------------------------------------------------------//
    double time = spec.get_option<double>(
            "time", {"t", "T"}, 1.0,
            "Simulation time."
            );

    //----------------------------------------------------------------------//
    json states = spec.get_option<json>(
            "graph", {"g", "G"}, states_default,
            "State graph"
            );

    //-----------------------------------------------------------------------//
    json efficiencies = spec.get_option<json>(
            "efficiencies", {"e", "E"}, efficiencies_default,
            "Transfer efficiencies"
            );

    //-----------------------------------------------------------------------//
    std::string start_at = spec.get_option<std::string>(
            "start-at", {"i", "I"}, states.begin()->at("from"),
            "Initial state."
            );

    //-----------------------------------------------------------------------//
    std::string output_name = spec.get_option<std::string>(
            "output", {"o", "O"}, "", 
            "Output"
            );


    spec.enable_help();
    spec.enable_config();
    spec.enable_log();
    spec.enable_debug();

    sim::io::Output<sim::TimedValue> output{output_name};

    // Building the graph

    sim::graph::Graph graph(seed);
    auto end_action = std::unique_ptr<sim::graph::Action> (new EndAction("end", graph));
    graph.add_action(end_action);
    
    //-----------------------------------------------------------------------//
    for (json::iterator it=states.begin(); it!=states.end(); ++it){
        json val = it.value();
        graph.add_node(val["from"]);
        graph.add_node(val["to"]);
        graph.add_edge(it.key(), val["from"], val["to"], val["rate"]);
    }

    //-----------------------------------------------------------------------//
    for (json::iterator it=efficiencies.begin(); it!=efficiencies.end(); ++it){

        std::string target_node_name = it.key();
        double const efficiency = it.value();

            std::unique_ptr<sim::graph::Action> fret_action_ptr{
                new FRETAction(
                        target_node_name,
                        graph,
                        target_node_name,
                        efficiency,
                        output
                        )
            };
            graph.add_action(fret_action_ptr);
            graph.link_node_action(target_node_name, target_node_name);
        }

    //-----------------------------------------------------------------------//
    graph.set_current(graph.get_node_ptr(start_at));
    graph.init();
    graph.push_event(sim::graph::Event(graph.get_action_ptr("end"), time));

    graph.traverse();

}
