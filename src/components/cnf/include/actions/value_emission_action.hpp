#include "graph/main.hpp"
#include "io/base_io.hpp"

namespace sim{
    namespace graph{

        class ValueEmissionAction : public Action{

            //---------------------------------------------------------------//
            // The value emissiona action emits a timed value when it is fired.
            // The time is taken from the graph's clock. The value is a 
            // constant associated with the action.
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                ValueEmissionAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        comp::Output<TimedValue> &out
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void set_value(double val);
                void set_trigger_node_id(std::string id);
                //-----------------------------------------------------------//

            private:

                //-----------------------------------------------------------//
                std::string trigger_node_id = "FO";
                double value = 0.0;

                //-----------------------------------------------------------//
                TimedValue current{0.0,0.0};
                comp::Output<TimedValue> &emission_output;

        };
    }
}
