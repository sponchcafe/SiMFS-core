#include "fixtures.hpp"


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddNode){
    g.add_node("S0");
    ASSERT_EQ(g.get_node_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultNode){
    g.add_node("S0");
    ASSERT_NE(g.get_node_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_node_ptr("nothere"), g.get_default_node_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetNodePointer){
    g.add_node("S0");
    ASSERT_NE(g.get_node_ptr("S0"), g.get_default_node_ptr());
    ASSERT_NE(g.get_node_ptr("S0"), nullptr);
    ASSERT_EQ(g.get_node_ptr("S0")->name, "S0");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateNode){
    g.add_node("S0");
    g.add_node("S0");
    ASSERT_EQ(g.get_node_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyNodes){
    g.add_node("S0");
    g.add_node("S1");
    g.add_node("S2");
    g.add_node("T0");
    ASSERT_EQ(g.get_node_count(), 4);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetNodeList){
    g.add_node("S0");
    g.add_node("S1");
    g.add_node("T1");
    std::string result = "";
    for(auto &node : g.get_nodes()){
        result += node.name;
    }
    ASSERT_EQ(result.length(), 6);
}



//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddEdge){
    g.add_edge("exi", "", "", 0);
    ASSERT_EQ(g.get_edge_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultEdge){
    g.add_edge("exi", "", "", 0);
    ASSERT_NE(g.get_edge_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_edge_ptr("nothere"), g.get_default_edge_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetEdgePointer){
    g.add_edge("exi", "", "", 0);
    ASSERT_NE(g.get_edge_ptr("exi"), g.get_default_edge_ptr());
    ASSERT_NE(g.get_edge_ptr("exi"), nullptr);
    ASSERT_EQ(g.get_edge_ptr("exi")->name, "exi");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateEdge){
    g.add_edge("exi", "", "", 0);
    g.add_edge("exi", "", "", 0);
    ASSERT_EQ(g.get_edge_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyEdges){
    g.add_edge("exi", "", "", 0);
    g.add_edge("emi", "", "", 0);
    g.add_edge("isc", "", "", 0);
    g.add_edge("risc", "", "", 0);
    ASSERT_EQ(g.get_edge_count(), 4);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetEdgeList){
    g.add_edge("exi", "", "", 0);
    g.add_edge("emi", "", "", 0);
    g.add_edge("isc", "", "", 0);
    g.add_edge("risc", "", "", 0);
    std::string result = "";
    for(auto &edge : g.get_edges()){
        result += edge.name;
    }
    ASSERT_EQ(result.length(), 13);
}



//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddAction){
    g.add_action(echo_action);
    ASSERT_EQ(g.get_action_count(), 1);
    ASSERT_EQ(echo_action.get(), nullptr);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultAction){
    ASSERT_NE(g.get_action_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_action_ptr("nothere"), g.get_default_action_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetActioPointer){
    g.add_action(echo_action);
    ASSERT_NE(g.get_action_ptr("echo_action"), g.get_default_action_ptr());
    ASSERT_NE(g.get_action_ptr("echo_action"), nullptr);
    ASSERT_EQ(g.get_action_ptr("echo_action")->name, "echo_action");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateAction){
    g.add_action(echo_action);
    std::unique_ptr<Action> a{new EchoAction(g, "echo_action", "y")};
    g.add_action(a);
    ASSERT_EQ(g.get_action_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyActions){
    g.add_action(echo_action);
    std::unique_ptr<Action> a{new EchoAction(g, "echo_action2", "y")};
    g.add_action(a);
    ASSERT_EQ(g.get_action_count(), 2);
}


