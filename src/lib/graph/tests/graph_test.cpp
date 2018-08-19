#include "fixtures.hpp"
#include <sstream>


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddNode){
    g.add_node("S0");
    g.init();
    ASSERT_EQ(g.get_node_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultNode){
    g.add_node("S0");
    g.init();
    ASSERT_NE(g.get_node_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_node_ptr("nothere"), g.get_default_node_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetNodePointer){
    g.add_node("S0");
    g.init();
    ASSERT_NE(g.get_node_ptr("S0"), g.get_default_node_ptr());
    ASSERT_NE(g.get_node_ptr("S0"), nullptr);
    ASSERT_EQ(g.get_node_ptr("S0")->name, "S0");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateNode){
    g.add_node("S0");
    g.add_node("S0");
    g.init();
    ASSERT_EQ(g.get_node_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyNodes){
    g.add_node("S0");
    g.add_node("S1");
    g.add_node("S2");
    g.add_node("T0");
    g.init();
    ASSERT_EQ(g.get_node_count(), 4);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetNodeList){
    g.add_node("S0");
    g.add_node("S1");
    g.add_node("T1");
    g.init();
    std::string result = "";
    for(auto &node : g.get_nodes()){
        result += node.name;
    }
    ASSERT_EQ(result.length(), 6);
}



//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddEdge){
    g.add_edge("exi", "", "", 0);
    g.init();
    ASSERT_EQ(g.get_edge_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultEdge){
    g.add_edge("exi", "", "", 0);
    g.init();
    ASSERT_NE(g.get_edge_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_edge_ptr("nothere"), g.get_default_edge_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetEdgePointer){
    g.add_edge("exi", "", "", 0);
    g.init();
    ASSERT_NE(g.get_edge_ptr("exi"), g.get_default_edge_ptr());
    ASSERT_NE(g.get_edge_ptr("exi"), nullptr);
    ASSERT_EQ(g.get_edge_ptr("exi")->name, "exi");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateEdge){
    g.add_edge("exi", "", "", 0);
    g.add_edge("exi", "", "", 0);
    g.init();
    ASSERT_EQ(g.get_edge_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyEdges){
    g.add_edge("exi", "", "", 0);
    g.add_edge("emi", "", "", 0);
    g.add_edge("isc", "", "", 0);
    g.add_edge("risc", "", "", 0);
    g.init();
    ASSERT_EQ(g.get_edge_count(), 4);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetEdgeList){
    g.add_edge("exi", "", "", 0);
    g.add_edge("emi", "", "", 0);
    g.add_edge("isc", "", "", 0);
    g.add_edge("risc", "", "", 0);
    g.init();
    std::string result = "";
    for(auto &edge : g.get_edges()){
        result += edge.name;
    }
    ASSERT_EQ(result.length(), 13);
}



//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddAction){
    g.add_action(echo_action);
    g.init();
    ASSERT_EQ(g.get_action_count(), 1);
    ASSERT_EQ(echo_action.get(), nullptr);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, DefaultAction){
    g.init();
    ASSERT_NE(g.get_action_ptr("nothere"), nullptr);
    ASSERT_EQ(g.get_action_ptr("nothere"), g.get_default_action_ptr());
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetActioPointer){
    g.add_action(echo_action);
    g.init();
    ASSERT_NE(g.get_action_ptr("echo_action"), g.get_default_action_ptr());
    ASSERT_NE(g.get_action_ptr("echo_action"), nullptr);
    ASSERT_EQ(g.get_action_ptr("echo_action")->name, "echo_action");
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddDuplicateAction){
    g.add_action(echo_action);
    std::unique_ptr<Action> a{new EchoAction(g, "echo_action", "y")};
    g.add_action(a);
    g.init();
    ASSERT_EQ(g.get_action_count(), 1);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, AddManyActions){
    g.add_action(echo_action);
    std::unique_ptr<Action> a{new EchoAction(g, "echo_action2", "y")};
    g.add_action(a);
    g.init();
    ASSERT_EQ(g.get_action_count(), 2);
}


//---------------------------------------------------------------------------//
TEST_F(GraphBuildTest, GetActionList){
    g.add_action(echo_action);
    std::unique_ptr<Action> a{new EchoAction(g, "echo_action2", "y")};
    g.add_action(a);
    g.init();
    std::string result = "";
    for(auto &action : g.get_actions()){
        result += action->name;
    }
    ASSERT_EQ(result.length(), 23);
}


//---------------------------------------------------------------------------//
TEST_F(GraphTraversalTest, InitializationLinks){
    g.init();
    Node * S0 = g.get_node_ptr("S0");
    Node * S1 = g.get_node_ptr("S1");
    Edge * exi = g.get_edge_ptr("exi");
    Edge * emi = g.get_edge_ptr("emi");
    Edge * ic = g.get_edge_ptr("ic");

    ASSERT_EQ(S0->get_edges()[0], g.get_edge_ptr("exi"));
    ASSERT_EQ(S1->get_edges()[0], g.get_edge_ptr("emi"));
    ASSERT_EQ(S1->get_edges()[1], g.get_edge_ptr("ic"));

    ASSERT_EQ(exi->get_target_node_ptr(), g.get_node_ptr("S1"));
    ASSERT_EQ(emi->get_target_node_ptr(), g.get_node_ptr("S0"));
    ASSERT_EQ(ic->get_target_node_ptr(), g.get_node_ptr("S0"));
}
    

//---------------------------------------------------------------------------//
TEST_F(GraphTraversalTest, InitializationActions){
    g.link_node_action("S1", "nothere");
    g.link_edge_action("exi", "echo_action");
    g.init();
    ASSERT_EQ(g.get_node_ptr("S0")->is_active(), false);
    ASSERT_EQ(g.get_node_ptr("S1")->is_active(), true);
    ASSERT_EQ(g.get_node_ptr("S1")->get_action_ptr(), 
            g.get_default_action_ptr());
    ASSERT_EQ(g.get_edge_ptr("exi")->is_active(), true);
    ASSERT_EQ(g.get_edge_ptr("emi")->is_active(), false);
}


//---------------------------------------------------------------------------//
TEST_F(GraphTraversalTest, RunSingleStep){
    g.link_node_action("S1", "nothere");
    g.link_edge_action("exi", "echo_action");
    g.set_current(g.get_node_ptr("S0"));
    g.init();
    std::stringstream ss;
    testing::internal::CaptureStderr();
    g.traverse();
    std::string output = testing::internal::GetCapturedStderr();
    ss << "e__default_action__ called from " << *g.get_node_ptr("S1") << " at ";
    std::string res = ss.str();
    ASSERT_EQ(output.substr(0, res.length()), res); 
}


//---------------------------------------------------------------------------//
TEST_F(GraphTraversalTest, RunSimulation){
    g.link_edge_action("emi", "echo_action");
    g.set_current(g.get_node_ptr("S0"));
    Event e{g.get_default_action_ptr(), 0.1};
    g.push_event(e);
    g.init();
    std::stringstream ss;
    testing::internal::CaptureStderr();
    g.traverse();
    std::string output = testing::internal::GetCapturedStderr();
    ss << "__default_action__ called from " << *g.get_current_ptr() << " at 0.1\n";
    std::string res = ss.str();
    ASSERT_EQ(output.substr(output.length()-res.length(), output.length()), ss.str());
}

//---------------------------------------------------------------------------//
TEST_F(GraphTraversalTest, EqualPaths){
    g.link_edge_action("emi", "echo_action");
    g.link_edge_action("ic", "echo_action2");
    g.init();
    g.set_current(g.get_node_ptr("S0"));
    Event e{g.get_default_action_ptr(), 1e-3};
    g.push_event(e);
    testing::internal::CaptureStderr();
    g.traverse();
    std::string output = testing::internal::GetCapturedStderr();
    int es = 0;
    int fs = 0;
    for (auto it: output){
        if (it == 'e') es++;
        if (it == 'f') fs++;
    }
    ASSERT_LT(abs(es-fs), (es+fs)/100);
}
