#include "graph/graph.hpp"

namespace sim{
    namespace graph{

        Edge::Edge(const std::string name, const std::string source_name, const std::string target_name, double lambda, unsigned seed) :
            lifetime(0.0), name(name), source_name(source_name), target_name(target_name), exp(random::Exponential(lambda, seed))
        {
        }

        void Edge::set_action(Action *a){
            if (a != nullptr){
                action = a;
            }
        }

        void Edge::calculate_lifetime(){
            lifetime = exp();
        }

        void Edge::set_lambda(double lambda){
            exp.set_lambda(lambda);
        }

        void Edge::traverse(){
            if (action != nullptr){
                action->fire();
            }
        }

        std::ostream &operator<<(std::ostream &os, const Edge &edge){
            os << "  " << "<Edge " << edge.name << " at " << &edge << " -> ";
            if (edge.target != nullptr) os << *edge.target;
            os << ">";
            if (edge.action != nullptr) {
                std::cerr << " => "; 
                edge.action->print_info(os);
            }
            os << "  " << std::endl;
            os << "  " << "  state=" << ((edge.active) ? "active":"inactive") << std::endl;
            os << "  " << "  lifetime=" << edge.lifetime << std::endl;
            os << "  " << "  lambda=" << edge.exp.get_lambda() << std::endl;
            return os;
        }
    }
}
