#include "fixtures.hpp"

TEST_F(GraphBuildTest, EmptyNodes){
    /* Assert that the default action warning is triggered when a 
     * non-exisiting node is traversed. */
    Node *n = g.get_node("nothere");
    testing::internal::CaptureStderr();
    n->traverse();
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_EQ(output.substr(0, 7), "Warning");
}

TEST_F(GraphBuildTest, EmptyEdges){
    /* Assert that the default action warning is triggered when a 
     * non-exisiting edge is traversed. */
    Edge *e = g.get_edge("nothere");
    testing::internal::CaptureStderr();
    e->traverse();
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_EQ(output.substr(0, 7), "Warning");
}

TEST_F(GraphBuildTest, EmptyActions){
    /* Assert that the default action warning is triggered when a 
     * non-exisiting action is fired. */
    Action *a = g.get_action("nothere");
    testing::internal::CaptureStderr();
    a->fire();
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_EQ(output.substr(0, 7), "Warning");
}

TEST_F(GraphBuildTest, AddNode){
    /* Add and retrieve node */
    g.add_node("S0");
    g.init("S0", "__default_action__"); // reinitialize necessary
    Node *n = g.get_node("S0");
    ASSERT_EQ(n->name, "S0");
}

TEST_F(GraphBuildTest, AddEdge){
    /* Add and retrieve edge */
    g.add_edge("exi", "S0", "S1", -1);
    g.init("S0", "__default_action__");
    Edge *e = g.get_edge("exi");
    ASSERT_EQ(e->name, "exi");
    ASSERT_EQ(e->source_name, "S0");
    ASSERT_EQ(e->target_name, "S1");
}

TEST_F(GraphBuildTest, AddAction){
    /* Add and retrive action */
    g.add_action(echo_action);
    g.init("S0", "__default_action__");
    Action *a = g.get_action("EchoAction");
    ASSERT_EQ(a->name, "EchoAction");
    testing::internal::CaptureStderr();
    a->fire();
    std::string output = testing::internal::GetCapturedStderr();
    ASSERT_EQ(output.substr(0, 4), "echo");
}

TEST_F(GraphBuildTest, EventBuild){
    g.add_action(echo_action);
    Event e = Event(g.get_action("EchoAction"), 1.0);
    ASSERT_EQ(e.time, 1.0);
    ASSERT_EQ(e.target, g.get_action("EchoAction"));
    ASSERT_NE(e.target, nullptr);
    Event e2{e};
    ASSERT_EQ(e2.time, 1.0);
    ASSERT_NE(e2.target, nullptr);
    ASSERT_EQ(e2.target, g.get_action("EchoAction"));
    g.push_event(e2);
}

/*
 * functional test disabled.
TEST_F(GraphLinkTest, Actions){
    ASSERT_EXIT(g.traverse(), [](int x){return x==0;}, "update");
}
*/


