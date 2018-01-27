#include "graph/action.hpp"

namespace sim{
    namespace graph{

        // ctor BaseAction
        Action::Action(const std::string name, Graph &graph) : name(name), graph(graph) {}

        void Action::init() {}

        // stream insertion operator
        std::ostream &Action::print_info(std::ostream &os){
            return os << "<Action " << name << " at " << this << ">";
        }


        DefaultAction::DefaultAction(Graph &graph) : Action("__default__", graph){};
        void DefaultAction::fire() {
            std::cerr << "Warning! The default action has been triggered. This possibly means that the graph\
                graph is not laid out as you expect it. The simulation will be terminated now." << std::endl;
            graph.done = true;
            graph.print_state();
        }
    }
}
