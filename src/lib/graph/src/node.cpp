#include "graph/rate_graph.hpp"
#include <limits>
#include <iostream>

namespace sim{
    namespace graph{

        Node::Node(const std::string name) : name(name) {}

        void Node::add_edge(Edge *edge){
            if (edge != nullptr){
                outgoing_edges.push_back(edge);
            }
        }

        void Node::set_action(Action *a){
            if (a != nullptr){
                action = a;
            }
        }
        
        Edge *Node::get_next(){
            double min_lifetime = std::numeric_limits<double>::max();
            Edge *next;
            for (auto it=outgoing_edges.begin(); it!=outgoing_edges.end(); ++it){
                Edge *e = *it;
                e->calculate_lifetime();
                if (e->lifetime < min_lifetime){
                    next = e;
                    min_lifetime = e->lifetime;
                }
            }
            return next;
        }

        void Node::traverse(){
            if (action != nullptr){
                action->fire();
            }
        }

        std::ostream &operator<<(std::ostream &os, const Node &node){
            os << "<Node " << node.name << " at " << &node << "> => ";
            if(node.action != nullptr) {
                node.action->print_info(os);
            }else{
                std::cerr << "<none>";
            }
            return os;
        }

        void Node::print_edges(std::ostream &os){
            for (auto it=outgoing_edges.begin(); it!=outgoing_edges.end(); ++it){
                os << **it;
            }
        }

    }
}
