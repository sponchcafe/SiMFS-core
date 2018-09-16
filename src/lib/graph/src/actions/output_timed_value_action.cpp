#include "actions/output_timed_value_action.hpp"

namespace sim{
    namespace graph{

        std::string OutputTimedValueAction::type = "output_timed_value";

        //-------------------------------------------------------------------//
        OutputTimedValueAction::OutputTimedValueAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::unique_ptr<io::BufferOutput<TimedValue>> &output) : 
            Action(graph, name),
            output_ptr(output){
            } 

        //-------------------------------------------------------------------//
        void OutputTimedValueAction::set_value(double val) {
            tvalue.value = val;
        }

        //-------------------------------------------------------------------//
        void OutputTimedValueAction::init() {
        }

        //-------------------------------------------------------------------//
        void OutputTimedValueAction::fire() {
            tvalue.time = graph.get_clock();
            output_ptr->put(tvalue);
        }

    }
}
