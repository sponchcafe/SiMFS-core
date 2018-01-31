#ifndef SIM_GRAPH_ACTION_H
#define SIM_GRAPH_ACTION_H

#include "graph/graph.hpp"
#include "graph/definitions.hpp"

#include <string>

namespace sim{
    namespace graph{

        class Action{

            public:

                //-----------------------------------------------------------//
                Action(Graph &graph, std::string const name);

                virtual void fire();
                virtual void init();

                std::ostream &print_info(std::ostream &os);

                std::string const name;

            protected:

                Graph &graph;

        };

    }
}

#endif
