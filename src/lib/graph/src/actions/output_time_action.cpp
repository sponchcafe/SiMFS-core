#include "actions/output_time_action.hpp"

namespace sim{
    namespace graph{

        std::string OutputTimeAction::type = "output_time";

        //-------------------------------------------------------------------//
        OutputTimeAction::OutputTimeAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::unique_ptr<io::BufferOutput<realtime_t>> &output) : 
            Action(graph, name),
            output_ptr(output) {
            } 

        //-------------------------------------------------------------------//
        void OutputTimeAction::init() {
        }

        //-------------------------------------------------------------------//
        void OutputTimeAction::fire() {
            double time = graph.get_clock();
            output_ptr->put(time);
        }

    }
}
