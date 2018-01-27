#ifndef SIM_RATE_GRAPH_ACTION_H
#define SIM_RATE_GRAPH_ACTION_H

#include <string>
#include "graph/rate_graph.hpp"

namespace sim{
    namespace graph{

        class Action{

            /* Pure abstract base class for actions occuring in a graph.
             * specifies fire and init
             * has name and graph reference
             */
            public:

                // ctor
                Action(const std::string name, Graph &graph);

                virtual void fire() = 0;
                void init();

                std::ostream &print_info(std::ostream &os);

                const std::string name;

            protected:

                Graph &graph;

        };

        class DefaultAction : public Action{

            public:

                DefaultAction(Graph &graph);
                void fire() override;

        };

    }
}

#endif
