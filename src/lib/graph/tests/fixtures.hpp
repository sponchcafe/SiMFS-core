#include "gtest/gtest.h"

#include "graph/main.hpp"

using namespace sim;
using namespace graph;


//---------------------------------------------------------------------------//
class EchoAction : public Action{

    public:

        //-------------------------------------------------------------------//
        EchoAction(Graph &graph,
                std::string const name,
                std::string const message)
            : Action(graph, name), message(message)
        {
        }

        
        //-------------------------------------------------------------------//
        void init() override {}

        //-------------------------------------------------------------------//
        void fire() override { std::cerr << message; }


    private:

        //-------------------------------------------------------------------//
        const std::string message;

};


//---------------------------------------------------------------------------//
class GraphBuildTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        GraphBuildTest() : g(Graph(0)){
            echo_action = std::unique_ptr<Action>{
                new EchoAction(g, "echo_action", "e")
            };
        }


        //-------------------------------------------------------------------//
        virtual void SetUp() {}
        
        
        //-------------------------------------------------------------------//
        virtual void TearDown() {}
        
        
        //-------------------------------------------------------------------//
        Graph g;
        std::unique_ptr<Action> echo_action;

};


//---------------------------------------------------------------------------//
class GraphTraversalTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        GraphTraversalTest() : g(Graph(0)){
            echo_action = std::unique_ptr<Action>{
                new EchoAction(g, "echo_action", "e")
            };
            echo_action2 = std::unique_ptr<Action>{
                new EchoAction(g, "echo_action2", "f")
            };
        }


        //-------------------------------------------------------------------//
        virtual void SetUp() {
            g.add_node("S0");
            g.add_node("S1");
            g.add_edge("exi", "S0", "S1", 1e+6);
            g.add_edge("emi", "S1", "S0", 1e+8);
            g.add_edge("ic", "S1", "S0", 1e+8);
            g.add_action(echo_action);
            g.add_action(echo_action2);
        }
        
        
        //-------------------------------------------------------------------//
        virtual void TearDown() {}
        
        
        //-------------------------------------------------------------------//
        Graph g;
        std::unique_ptr<Action> echo_action;
        std::unique_ptr<Action> echo_action2;

};
