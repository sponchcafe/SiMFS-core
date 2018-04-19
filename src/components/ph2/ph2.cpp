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

class TransferAction : public sim::graph::Action{
    
    //-----------------------------------------------------------------------//
    // The transfer action consumes photon timetags from its input and issues
    // excitation events for its target edge. When the event is fired and the
    // graph is in an excitable state, an excitation state transtition is
    // performed. If the graph is however not in an excitable state, the photon
    // transfer is rejected and the events timetag is put to the rejected
    // output and can be mixed back into the donor emission stream.
    //-----------------------------------------------------------------------//

    public: 

        //-------------------------------------------------------------------//
        TransferAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::string const target_edge_name,
                std::string const source_node_name,
                std::string const input_name,
                std::string const rejected_output_name
                ) :
            Action(graph, name), 
            target_edge_name(target_edge_name),
            source_node_name(source_node_name),
            input(sim::io::Input<sim::realtime_t>(input_name)),
            rejected_output(sim::io::Output<sim::realtime_t>(rejected_output_name))
        {
        }

        //-------------------------------------------------------------------//
        void init() override {
            target_edge = graph.get_edge_ptr(target_edge_name);
            source_node = graph.get_node_ptr(source_node_name);
            input.get(current);
            graph.push_event(sim::graph::Event(this, current)); 
        }
        
        //-------------------------------------------------------------------//
        void fire() override {

            if (graph.get_current_ptr() == source_node) {
                // excitation accepted
                graph.set_current(target_edge->get_target_node_ptr());
            }
            else{
                // excitation rejetcted
                rejected_output.put(current);
            }

            if (input.get(current)){
                graph.push_event(sim::graph::Event(this, current)); 
            }

        }

    private:

        std::string const target_edge_name;
        sim::graph::Edge *target_edge = nullptr;

        std::string const source_node_name;
        sim::graph::Node *source_node = nullptr;

        sim::io::Input<sim::realtime_t>input;
        sim::io::Output<sim::realtime_t>rejected_output;

        sim::realtime_t current;

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

json actions_default =  
{
    {"excitation", {{"type", "excitation"}, {"target-edge", "exi"}, {"input", ""}}},
    {"emission", {{"type", "emission"}, {"target-edge", "emi"}, {"output", ""}}},
    {"transfer", 
        {
            {"type", "transfer"}, 
            {"target-edge", "exi"}, 
            {"source-node", "S0"}, 
            {"input", ""}, 
            {"output", ""}
        }
    }
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
            "start-at", {"i", "I"}, jablonsky.begin()->at("from"),
            "Initial state."
            );

    //----------------------------------------------------------------------//
    json actions = spec.get_option<json>(
            "actions", {"a", "A"}, actions_default,
            "Actions"
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
    for (json::iterator it=actions.begin(); it!=actions.end(); ++it){

        json val = it.value();
        std::string type = val["type"];
        std::string name = it.key();

        if (type == "excitation"){
            std::unique_ptr<sim::graph::Action> ex_act_ptr{
                new ExcitationAction(
                        name,
                        graph,
                        val["target-edge"], // target edge name
                        jablonsky[val["target-edge"].get<std::string>()]["epsilon"], // abs coef
                        val["input"] // input stream
                        )
            };
            graph.add_action(ex_act_ptr);
        }

        else if(type == "transfer"){
            std::unique_ptr<sim::graph::Action> transfer_act_ptr{
                new TransferAction(
                        name,
                        graph,
                        val["target-edge"], // path to excite
                        val["source-node"], // precondition state
                        val["input"], // input stream
                        val["output"] // output for rejected photons
                        )
            };
            graph.add_action(transfer_act_ptr);
        }

        else if (type == "emission"){
            std::unique_ptr<sim::graph::Action> emi_act_ptr{
                new EmissionAction(
                        name,
                        graph,
                        val["output"]
                        )
            };
            graph.add_action(emi_act_ptr);
            graph.link_edge_action(val["target-edge"], it.key());
        }

    }

    //-----------------------------------------------------------------------//
    graph.set_current(graph.get_node_ptr(start_at));
    graph.init();

    graph.traverse();

}
