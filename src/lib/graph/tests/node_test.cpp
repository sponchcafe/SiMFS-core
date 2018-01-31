#include "gtest/gtest.h"
#include "fixtures.hpp"
#include "graph/rate_graph.hpp"

using namespace sim;
using namespace graph;

//-Node-creation------------------------------------------------------------//
TEST(NodeTest, Creation){
    Node n = Node(g, "S0");
    ASSERT_EQ(n.get_action(), g.get_default_action);
}

