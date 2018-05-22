#include "graph/action.hpp"
#include "graph/node.hpp"

#include <iostream>

namespace sim{
    namespace graph{

        std::string Action::type = "__default__";

        //-------------------------------------------------------------------//
        Action::Action(Graph &graph, std::string const name) 
            : name(name), graph(graph)
        {}

        //-------------------------------------------------------------------//
        Action::~Action()
        {}

        //-------------------------------------------------------------------//
        void Action::init(){}

        //-------------------------------------------------------------------//
        void Action::fire(){
            graph.set_done(true);
            std::cerr << name;
            std::cerr << " called from " << graph.get_current_ptr()->name;
            std::cerr << " at " << graph.get_clock() << std::endl;
        }

        //-------------------------------------------------------------------//
        void Action::set_json(json j){}

        //-------------------------------------------------------------------//
        json Action::get_json(){
            return json{};
        }

        //-------------------------------------------------------------------//
        std::ostream &Action::print_info(std::ostream &os){
            return os << "<Action " << name << " at " << this << ">";
        }

        //-------------------------------------------------------------------//
        bool action_type_compare(std::string lhs, std::string rhs){
            return lhs.compare(rhs) == 0;
        }

    }
}
