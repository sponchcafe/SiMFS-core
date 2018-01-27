#ifndef SIM_RATE_GRAPH_NODE_H
#define SIM_RATE_GRAPH_NODE_H

#include <string>
#include <vector>
#include "graph/rate_graph.hpp"

namespace sim{
    namespace graph{

        class Node{

            public:

                // ctor
                Node(const std::string name);

                void traverse();
                void add_edge(Edge *edge);
                void set_action(Action *a);
                Edge *get_next();

                const std::string name;

                void print_edges(std::ostream &os);
                friend std::ostream &operator<<(std::ostream &os, const Node &node);

            private:

                std::vector<Edge *> outgoing_edges;
                Action *action = nullptr;

        };


    }
}

#endif
