#ifndef SIM_GRAPH_H
#define SIM_GRAPH_H

#include <string>
#include <vector>
#include <queue>
#include "graph/rate_graph.hpp"

namespace sim{
    namespace graph{

        class Graph{

            public:

                typedef double graph_time_t;

                Graph(unsigned seed);
                void print_state();

                void add_node(const std::string name);
                void add_edge(const std::string name, const std::string source, const std::string target, double rate = 0);
                void add_action(Action *action);

                void link_node_action(const std::string action, const std::string node);
                void link_edge_action(const std::string action, const std::string edge);

                void init(const std::string startnode_name, const std::string startaction_name);

                void traverse();
                void execute_next_event();
                void push_event(Event e);

                Node *get_node(const std::string name);
                Edge *get_edge(const std::string name);
                Action *get_action(const std::string name);

                graph_time_t clock = 0.0;
                bool done = false;

            private:

                unsigned get_new_seed();

                std::unique_ptr<Node> default_node;
                std::unique_ptr<Edge> default_edge;
                std::unique_ptr<Action> default_action;

                Node *startnode = nullptr;
                Action *startaction = nullptr;

                void print_actions(size_t indent);
                void print_events(size_t indent);
                void print_nodes(size_t indent);

                Node *current = nullptr;
                std::vector<Node> nodes;
                std::vector<Edge> edges;
                std::vector<std::unique_ptr<Action>> actions;
                std::priority_queue<Event> events;
                
                unsigned seed;

        };

    }
}


#endif
