#include "gtest/gtest.h"
#include "graph/rate_graph.hpp"
#include "sim/io.hpp"
#include "sim/types.hpp"
#include "iostream"

using namespace sim;
using namespace graph;



class EchoAction : public Action{
    public:
        EchoAction(const std::string name, const std::string message, Graph &graph)
            : Action(name, graph), message(message)
        {
        }
        void fire() override {
            std::cerr << message << " @ " << graph.clock << std::endl;
        }
    private:
        const std::string message;
};

class StopAction : public Action{
    public:
        StopAction(const std::string name, Graph &graph)
            : Action(name, graph)
        {
        }
        void fire() override {
            graph.done = true;
            std::cerr << "STOPP ACTION\n";
        }
    private:
        const std::string message;
};

class StartAction : public Action{
    public:
        StartAction(const std::string name, Graph &graph)
            : Action(name, graph)
        {
        }
        void fire() override {
            Event e1 = Event(graph.get_action("update"), 0.0);
            Event e = Event(graph.get_action("stop"), 0.001);
            graph.push_event(e1);
            graph.push_event(e);
        }
    private:
        const std::string message;
};

class UpdateAction: public Action{
    public:
        UpdateAction(const std::string name, Graph &graph, double interval)
            : Action(name, graph), interval(interval)
        {
        }
        void fire() override {
            Event e = Event(this, graph.clock+interval);
            graph.push_event(e);
        }

    private:
        double interval = 0.0;
        const std::string message;
};



/* =========== */

class GraphBuildTest: public ::testing::Test{
    protected:
        GraphBuildTest() : g(Graph(0)){
            echo_action = new EchoAction("EchoAction", "echo", g);
        }
        virtual void SetUp() {}
        virtual void TearDown(){}
        Graph g;
        EchoAction *echo_action;
};

class GraphLinkTest: public ::testing::Test{
    protected:
        
        GraphLinkTest() : g(Graph(0)){

            Action *emission = new EchoAction("emission", "emitting", g);
            g.add_action(emission);
            Action *start = new StartAction("start", g);
            g.add_action(start);
            Action *stop = new StopAction("stop", g);
            g.add_action(stop);
            Action *update = new UpdateAction("update", g, 1e-7);
            g.add_action(update);

            g.add_node("S0");
            g.add_node("S1");

            g.add_edge("exi", "S0", "S1", 1e+5);
            g.add_edge("emi", "S1", "S0", 1e+8);

            g.link_edge_action("emission", "emi");
            g.init("S0", "start");
        }
        virtual void SetUp() {}
        virtual void TearDown(){}
        Graph g;
};
