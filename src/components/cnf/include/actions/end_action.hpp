#include "graph/main.hpp"
#include "io/base_io.hpp"

namespace sim{
    namespace graph{

        class EndAction : public Action{

            //---------------------------------------------------------------//
            // The end action terminates the simulation by setting the graph's
            // done flag.
            //---------------------------------------------------------------//

            public: 

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                EndAction(
                        std::string const name,
                        sim::graph::Graph &graph
                        );

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void fire() override;

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

            private:


        };
    }
}
