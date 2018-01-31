#include "gtest/gtest.h"

#include "graph/main.hpp"

using namespace sim;
using namespace graph;


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
