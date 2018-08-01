#include "graph/main.hpp" 
#include "io/buffer.hpp"

namespace sim{
    namespace graph{

        class SetRatesAction : public Action{

            //---------------------------------------------------------------//
            //---------------------------------------------------------------//

            public:

                static std::string type;

                //-----------------------------------------------------------//
                SetRatesAction(
                        std::string const name,
                        sim::graph::Graph &graph,
                        std::unique_ptr<io::BufferInput<TimedValue>> &input
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void set_target_edge_ids(std::vector<std::string> &ids);
                void set_scalings(std::vector<double> &scalings);
                //-----------------------------------------------------------//

            private:

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<TimedValue>> &input_ptr;
                std::vector<std::string> target_edge_ids{};
                std::vector<double> scalings{};
                std::vector<sim::graph::Edge *> targets;
                sim::TimedValue current{0.0, 0.0};

        };

    }
}
