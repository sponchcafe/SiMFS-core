#include "actions/end_action.hpp"

namespace sim{
    namespace graph{

        std::string EndAction::type = "end";

        //-------------------------------------------------------------------//
        EndAction::EndAction(
                std::string const name,
                sim::graph::Graph &graph) :
            Action(graph, name) { } 

        //-------------------------------------------------------------------//
        void EndAction::set_json(json j) { }

        //-------------------------------------------------------------------//
        json EndAction::get_json() { return json{}; }

        //-------------------------------------------------------------------//
        void EndAction::init() { }

        //-------------------------------------------------------------------//
        void EndAction::fire() {
            graph.set_done(true);
        }

    }
}
