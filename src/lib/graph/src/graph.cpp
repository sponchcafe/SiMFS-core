#include <iostream>
#include "graph/graph.hpp"

namespace sim{
    namespace graph{

        Graph::Graph(unsigned seed) : seed(seed) {
            default_action = std::unique_ptr<Action>{new DefaultAction(*this)};
            default_node = std::unique_ptr<Node>(new Node("__default_node__"));
            default_edge = std::unique_ptr<Edge>(new Edge("__default_edge__", "__default_node__", "__default_node__", 0, 0));
            default_node->add_edge(default_edge.get()); 
            default_edge->target = default_node.get();
            default_node->set_action(default_action.get());
            default_edge->set_action(default_action.get());
        }

        void Graph::print_state(){
            std::cerr << "STATE REPORT GRAPH @" << this << std::endl;
            std::cerr << "==================================" << std::endl;
            std::cerr << std::endl;
            std::cerr << "ACTIONS" << std::endl;
            std::cerr << "-------" << std::endl;
            std::cerr << std::endl;
            print_actions(4);
            std::cerr << std::endl;
            std::cerr << "EVENTS" << std::endl;
            std::cerr << "------" << std::endl;
            std::cerr << std::endl;
            print_events(4);
            std::cerr << std::endl;
            std::cerr << "STATES:" << std::endl;
            std::cerr << "------" << std::endl;
            std::cerr << std::endl;
            print_nodes(4);
            std::cerr << std::endl;
            std::cerr << "=======" << std::endl;
        }

        void Graph::print_actions(size_t indent){
            std::string ind(indent, ' ');
            for (auto it=actions.begin(); it!= actions.end(); ++it){
                Action *a = it->get();
                a->print_info(std::cerr);
                std::cerr << std::endl;
            }
        }
        void Graph::print_events(size_t indent){
            std::string ind(indent, ' ');
            std::cerr << ind << "Clock: " << clock << std::endl;
            std::cerr << ind << "Done: " << ((done) ? "yes" : "no") << std::endl;
            if (!events.empty()){
                Event e = events.top();
                std::cerr << ind << "Next event: " << e << std::endl;
            }
            std::cerr << ind << "Total pending: " << events.size() << std::endl;
        
        }
        void Graph::print_nodes(size_t indent){
            std::string ind(indent, ' ');
            for (auto it=nodes.begin(); it!= nodes.end(); ++it){
                std::cerr << std::endl;
                std::cerr << *it << std::endl;
                it->print_edges(std::cerr);
            }
        }

        unsigned Graph::get_new_seed(){
            return seed++;
        }

        void Graph::add_node(const std::string name){
            Node n = Node(name);
            nodes.push_back(n);
        }

        void Graph::add_edge(const std::string name, const std::string source_name, const std::string target_name, double lambda){
            Edge e = Edge(name, source_name, target_name, lambda, get_new_seed());
            edges.push_back(e);
        }

        void Graph::add_action(Action *action){
            std::unique_ptr<Action> uptr(action);
            actions.push_back(std::move(uptr)); 
        }

        void Graph::link_node_action(const std::string action_name, const std::string node_name){
            Node *node = get_node(node_name);
            Action *action = get_action(action_name);
            node->set_action(action);
        }

        void Graph::link_edge_action(const std::string action_name, const std::string edge_name){
            Edge *edge = get_edge(edge_name);
            Action *action = get_action(action_name);
            edge->set_action(action);
        }

        Node *Graph::get_node(const std::string name){
            for (auto it=nodes.begin(); it!=nodes.end(); ++it){
                if (it->name == name){
                    return &(*it);
                }
            }
            return default_node.get();
        }

        Edge *Graph::get_edge(const std::string name){
            for (auto it=edges.begin(); it!=edges.end(); ++it){
                if (it->name == name){
                    return &(*it);
                }
            }
            return default_edge.get();
        }

        Action *Graph::get_action(const std::string name){
            for (auto it=actions.begin(); it!=actions.end(); ++it){
                Action *a = it->get();
                if (a->name == name){
                    return it->get();
                }
            }
            return default_action.get();
        }

        void Graph::init(const std::string startnode_name, const std::string startaction_name){
            // connect all nodes
            for (auto it=edges.begin(); it!=edges.end(); ++it){
                Node *source_node = get_node(it->source_name);
                Node *target_node = get_node(it->target_name);
                source_node->add_edge(&(*it));
                it->target = target_node;
            }
            // set start node
            startnode = get_node(startnode_name);
            startaction = get_action(startaction_name);
            current = startnode;

            // initialize actions
            for (auto it=actions.begin(); it!=actions.end(); ++it){
                (*it)->init();
            }

            // set start action
            startaction->fire(); // e.g. first rate update and issue event for first increment
        }

        void Graph::push_event(Event e){
            events.push(e);
        }

        void Graph::execute_next_event(){
            Event e = events.top();
            clock = events.top().time; // fast forward to event
            e.target->fire(); // executing event
            events.pop(); // remove event
        }

        void Graph::traverse() {

            Edge *next_path = default_edge.get();

            // Simulation mainloop
            while (true){

                current->traverse(); // will set done when appropriate

                if (done) break;

                next_path = current->get_next();
                while(!events.empty() && clock + next_path->lifetime >= events.top().time){
                    execute_next_event();
                    current->get_next(); // new try for lifetime
                }

                // path found, traversal ok
                clock += next_path->lifetime;
                next_path->traverse();
                current = next_path->target;

            }
                
        }

    
    }
}

