#include "catch.hpp"
#include "graph/main.hpp"
#include <sstream>


using namespace sim::graph;

/*---------------------------------------------------------------------------*/
TEST_CASE("Empty graph", "[graph]"){

    GIVEN("An empty graph"){

        Graph g{0};
        g.init();

        THEN("All counts are 0 (node, edge, action)"){

            REQUIRE(g.get_node_count() == 0);
            REQUIRE(g.get_edge_count() == 0);
            REQUIRE(g.get_action_count() == 0);

        }

        THEN("Non existing nodes map to the default node"){

            REQUIRE(g.get_default_node_ptr() != nullptr);
            REQUIRE(g.get_node_ptr("N1") == g.get_default_node_ptr());

        }

        THEN("Non existing edges map to the default edge"){

            REQUIRE(g.get_default_edge_ptr() != nullptr);
            REQUIRE(g.get_edge_ptr("E1") == g.get_default_edge_ptr());

        }

        THEN("Non existing actions map to the default action"){

            REQUIRE(g.get_default_action_ptr() != nullptr);
            REQUIRE(g.get_action_ptr("A1") == g.get_default_action_ptr());

        }

        THEN("The default loop is connected"){

            auto default_node = g.get_default_node_ptr();
            auto default_edge = g.get_default_edge_ptr();
            auto default_action = g.get_default_action_ptr();
            auto outgoing_edges = default_node->get_edges();

            REQUIRE(outgoing_edges.size() == 1);
            REQUIRE(outgoing_edges[0] == default_edge);
            REQUIRE(outgoing_edges[0]->get_target_node_ptr() == default_node);
            REQUIRE(outgoing_edges[0]->get_action_ptr() == default_action);
            REQUIRE(default_node->get_action_ptr() == default_action);

        }

    }

}

/*---------------------------------------------------------------------------*/
TEST_CASE("Node adding", "[graph]"){

    GIVEN("An empty graph"){

        Graph g{0};

        WHEN("A node is added"){

            g.add_node("N1");

            THEN("The node count increases"){

                REQUIRE(g.get_node_count() == 1);

                AND_THEN("A node pointer can be retrieved"){

                    REQUIRE(g.get_node_ptr("N1") != nullptr);
                    REQUIRE(g.get_node_ptr("N1") != g.get_default_node_ptr());
                    REQUIRE(g.get_node_ptr("N1")->name == "N1"); 

                }

            }

            WHEN("A duplicate node is added"){

                g.add_node("N1");

                THEN("The node count does not increase"){

                    REQUIRE(g.get_node_count() == 1);

                }

            }

            WHEN("A new node is added"){

                g.add_node("N2");

                THEN("The node count increases"){

                    REQUIRE(g.get_node_count() == 2);

                }

            }

        }

    }

}

/*---------------------------------------------------------------------------*/
TEST_CASE("Edge adding", "[graph]"){

    GIVEN("An empty graph"){

        Graph g{0};

        WHEN("An edge is added"){

            g.add_edge("E1", "", "", 0);

            THEN("The edge count increases"){

                REQUIRE(g.get_edge_count() == 1);

                AND_THEN("An edge pointer can be retrieved"){

                    REQUIRE(g.get_edge_ptr("E1") != nullptr);
                    REQUIRE(g.get_edge_ptr("E1") != g.get_default_edge_ptr());
                    REQUIRE(g.get_edge_ptr("E1")->name == "E1"); 

                }

            }

            WHEN("A dublicate edge is added"){

                g.add_edge("E1", "", "", 0);

                THEN("The edge count does not increase"){

                    REQUIRE(g.get_edge_count() == 1);

                }

            }

            WHEN("A new edge is added"){

                g.add_edge("E2", "", "", 0);

                THEN("The edge count increases"){

                    REQUIRE(g.get_edge_count() == 2);

                }

            }

        }

    }

}

/*-Dummy-action--------------------------------------------------------------*/
class LogAction : public Action {

    public:

        LogAction(Graph &graph,
                std::string const name,
                std::string const msg, 
                std::ostream &st) : 
            Action(graph, name), message{msg}, stream{st}
        {}

        void init() override { message = message + "_init"; }
        void fire() override { stream << message << '\n'; }

    private:

        std::string message;
        std::ostream &stream;

};

/*---------------------------------------------------------------------------*/
/* Move to action tests
 */
TEST_CASE("Action factory", "[action]"){

    Graph g{0};
    std::stringstream log{};
    auto uptr = make_action_uptr<LogAction>(
        std::ref(g), "logger", "fired", std::ref(log)
    );

    WHEN("The action is fired"){

        uptr->fire();

        THEN("A line is written to the log"){

            std::string l;
            log >> l;
            REQUIRE(l == "fired");

        }

    }

}

/*---------------------------------------------------------------------------*/
TEST_CASE("Action adding", "[graph]"){

    GIVEN("An empty graph"){

        Graph g{0};

        WHEN("A log action is added"){

            std::stringstream log{};
            g.add_action<LogAction>(std::ref(g), "logger", "fired", std::ref(log));

            THEN("The action count increases"){

                REQUIRE(g.get_action_count() == 1);

                AND_THEN("An action pointer can be retrieved"){

                    Action *act_ptr = g.get_action_ptr("logger");
                    REQUIRE(act_ptr != nullptr);
                    REQUIRE(act_ptr != g.get_default_action_ptr());
                    REQUIRE(act_ptr->name == "logger"); 

                    WHEN("The action is fired"){

                        act_ptr->fire();

                        THEN("A log entry is written"){

                            std::string l;
                            log >> l;
                            REQUIRE(l == "fired");

                        }

                    }

                    WHEN("The graph is initialized"){

                        g.init();

                        THEN("The action is initialized as well"){

                            act_ptr->fire(); 
                            std::string l;
                            log >> l;
                            REQUIRE(l == "fired_init");

                        }

                    }

                }


                WHEN("A duplicate action is added"){

                    g.add_action<LogAction>(std::ref(g), "logger", "fired", std::ref(log));

                    THEN("The action count does not increase"){

                        REQUIRE(g.get_action_count() == 1);

                    }

                }

                WHEN("A new action is added"){

                    g.add_action<LogAction>(std::ref(g), "logger2", "fired2", std::ref(log));

                    THEN("The action count does increase"){

                        REQUIRE(g.get_action_count() == 2);

                    }

                    WHEN("Both actions are fired"){

                        g.get_action_ptr("logger")->fire();
                        g.get_action_ptr("logger2")->fire();

                        THEN("Two log lines are written"){

                            std::string l1;
                            std::string l2;
                            log >> l1;
                            log >> l2;
                            REQUIRE(l1 == "fired");
                            REQUIRE(l2 == "fired2");

                        }

                    }

                }

            }

        }

    }

}

/*---------------------------------------------------------------------------*/
TEST_CASE("Node linking", "[graph][link]"){
    
    GIVEN("A graph with two nodes, one connecting edge and one action"){

        Graph g{0};
        g.add_node("N1");
        g.add_node("N2");
        g.add_edge("E1", "N1", "N2", 1);
        std::stringstream log{};
        g.add_action<LogAction>(std::ref(g), "logger", "fired", std::ref(log));

        WHEN("Initialized"){

            g.init();

            THEN("The nodes are connected"){

                auto node_edges = g.get_node_ptr("N1")->get_edges();
                REQUIRE(node_edges.size() == 1);

                auto edge1 = g.get_edge_ptr("E1");
                REQUIRE(node_edges[0] == edge1);

                auto node2 = g.get_node_ptr("N2");
                REQUIRE(node_edges[0]->get_target_node_ptr() == node2);

            }

        }

        WHEN("The action is linked to the edge"){

            g.link_edge_action("E1", "logger");

            WHEN("Initialized"){

                g.init();

                THEN("The action is linked to the edge"){

                    auto action = g.get_action_ptr("logger");
                    auto edge = g.get_edge_ptr("E1");
                    REQUIRE(edge->get_action_ptr() == action);


                }
            }

        }

        WHEN("The action and linked to a node"){

            g.link_node_action("N1", "logger");

            WHEN("Initialized"){

                g.init();

                THEN("The action is linked to the node"){

                    auto action = g.get_action_ptr("logger");
                    auto node = g.get_node_ptr("N1");
                    REQUIRE(node->get_action_ptr() == action);

                }
            }

        }

    }

}
