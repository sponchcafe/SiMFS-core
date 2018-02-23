#include "graph/action.hpp"
#include "graph/node.hpp"

#include <iostream>

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        Action::Action(Graph &graph, std::string const name) 
            : name(name), graph(graph)
        {}


        //-------------------------------------------------------------------//
        void Action::init() 
        {}


        //-------------------------------------------------------------------//
        void Action::fire() {
            graph.set_done(true);
            std::cerr  << name;
            std::cerr << " called from " << graph.get_current_ptr()->name;
            std::cerr << " at " << graph.get_clock() << std::endl;
        }


        //-------------------------------------------------------------------//
        std::ostream &Action::print_info(std::ostream &os){
            return os << "<Action " << name << " at " << this << ">";
        }

    }
}