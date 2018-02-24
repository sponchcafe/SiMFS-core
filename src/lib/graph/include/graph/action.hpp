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

                //-----------------------------------------------------------//
                // NOTE: A virtual destructor is necessary in an abstract
                // class to ensure the destructor of derived classes can be
                // invoked properly. In this case this is necessary to
                // trigger the stream flushing on destruction of actions 
                // owning a io stream.
                //-----------------------------------------------------------//
                virtual ~Action();

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
