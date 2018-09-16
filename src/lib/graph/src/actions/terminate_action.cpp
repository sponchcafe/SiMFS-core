#include "actions/terminate_action.hpp"

namespace sim{
    namespace graph{

        std::string TerminateAction::type = "terminate";

        //-------------------------------------------------------------------//
        TerminateAction::TerminateAction(
                std::string const name,
                sim::graph::Graph &graph) :
            Action(graph, name) { } 

        //-------------------------------------------------------------------//
        void TerminateAction::init() { }

        //-------------------------------------------------------------------//
        void TerminateAction::fire() {
            graph.set_done(true);
        }

    }
}
