#ifndef SIM_RATE_GRAPH_EDGE_H
#define SIM_RATE_GRAPH_EDGE_H

#include <string>
#include "graph/rate_graph.hpp"
#include "random/random.hpp"

namespace sim{
    namespace graph{

        class Edge{

            public: 

                // ctor
                Edge(
                        const std::string name,
                        const std::string source_name, 
                        const std::string target_name, 
                        double lambda,
                        unsigned seed
                    );

                void traverse();
                void calculate_lifetime();
                void set_action(Action *a);
                void set_lambda(double lambda);

                double lifetime;
                bool active = true;
                Node *target;
                
                void print_info(size_t indent);

                const std::string name;
                const std::string source_name;
                const std::string target_name;

                friend std::ostream &operator<<(std::ostream &os, const Edge &edge);

            private:

                Action *action = nullptr;
                random::Exponential exp;

        };

    }
}

#endif
