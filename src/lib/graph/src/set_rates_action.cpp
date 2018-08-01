#include "actions/set_rates_action.hpp"

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        std::string SetRatesAction::type = "set_rates";

        //-------------------------------------------------------------------//
        SetRatesAction::SetRatesAction(
                std::string const name,
                sim::graph::Graph &graph,
                std::unique_ptr<io::BufferInput<TimedValue>> &input) :
            Action(graph, name),
            input_ptr(input){
            }

        //-------------------------------------------------------------------//
        void SetRatesAction::set_target_edge_ids(std::vector<std::string> &ids){
            target_edge_ids = ids;
        }
        void SetRatesAction::set_scalings(std::vector<double> &scls){
            scalings = scls;
        }
        //-------------------------------------------------------------------//
        

        //-------------------------------------------------------------------//
        void SetRatesAction::init() {
            for (auto &id: target_edge_ids){
                targets.push_back(graph.get_edge_ptr(id));
            }
            //scaling = absorption_coefficient * 100.0/1000 * log(10) / CONST_NA;
            input_ptr->get(current);
            fire();
        }

        //-------------------------------------------------------------------//
        void SetRatesAction::fire() {

            auto scaling = scalings.begin();
            for (auto target = targets.begin(); target != targets.end(); ++target){
                auto target_ptr = *target;
                target_ptr->set_lambda(*scaling * current.value);
            }

            graph.push_event(Event(this, input_ptr->peek().time));

            if (!input_ptr->get(current)){
                graph.set_done(true);
                return;
            }

        }

    }
}
