#include "actions/heartbeat_action.hpp"

namespace sim{
    namespace graph{

        std::string HeartbeatAction::type = "output_time";

        //-------------------------------------------------------------------//
        HeartbeatAction::HeartbeatAction(
                std::string const name,
                sim::graph::Graph &graph,
                double interval,
                std::unique_ptr<io::BufferOutput<realtime_t>> &output) : 
            Action(graph, name),
            interval(interval),
            output_ptr(output) {
            } 

        //-------------------------------------------------------------------//
        void HeartbeatAction::init() {
            graph.push_event(graph::Event(this, -0));
        }

        //-------------------------------------------------------------------//
        void HeartbeatAction::fire() {
            double time = -graph.get_clock();
            output_ptr->put(time); // negative indicates heartbeat = no photon
            if (!graph.is_done()){
                graph.push_event(graph::Event(this, -time+interval));
            }

        }

    }
}
