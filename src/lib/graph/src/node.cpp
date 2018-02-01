#include "graph/node.hpp"
#include "graph/edge.hpp"

#include <limits>

namespace sim{
    namespace graph{

        //-CONSTRUCTOR-------------------------------------------------------//
        Node::Node(Graph &graph, std::string const name) 
            : name(name), graph(graph)
        {}


        //-Setup:-Add-an-edge-name-------------------------------------------//
        void Node::add_edge(std::string const edge_name){
            outgoing_edge_names.push_back(edge_name);
        }


        //-Get-edege-names---------------------------------------------------//
        std::vector<std::string const> const &Node::get_edge_names() {
            return outgoing_edge_names;
        }

        
        //-Get-edege-pointers------------------------------------------------//
        std::vector<Edge *> const &Node::get_edges() {
            return outgoing_edge_ptrs;
        }


        //-Setup:-Linked-action----------------------------------------------//
        void Node::set_action(std::string const action_name) {
            this->action_name = action_name;
        }

        
        //-Get-action-name---------------------------------------------------//
        std::string const Node::get_action_name() {
            return action_name;
        }


        //-Get-action-pointer------------------------------------------------//
        Action * Node::get_action_ptr() {
            return action_ptr;
        }


        //-------------------------------------------------------------------//
        bool Node::is_active() {
            return action_ptr != nullptr;
        }


        //-Initialization----------------------------------------------------//
        void Node::init(){

            // init action, EMPTY_NAME -> action = nullptr (no action)
            if (action_name != EMPTY_NAME){
                action_ptr = graph.get_action_ptr(action_name);
            }

            // init edge pointers
            for (auto &outgoing_edge_name: outgoing_edge_names){
                outgoing_edge_ptrs.push_back(
                        graph.get_edge_ptr(outgoing_edge_name)
                        );
            }
            
        }
        

        //-Get-next-path-to-traverse-----------------------------------------//
        Edge * Node::get_next(){

            // init lifetime to max and path to null
            double min_lifetime = std::numeric_limits<double>::max();
            Edge *next = graph.get_default_edge_ptr();

            // iterate edges, find smallest lifetime
            for (auto &edge_ptr: outgoing_edge_ptrs){

                double current_lifetime = edge_ptr->calculate_lifetime();

                if (current_lifetime < min_lifetime){
                    next = edge_ptr;
                    min_lifetime = current_lifetime;
                }        

            }

            // Return next path or the graphs default edge when edge is null.
            // When the node has no outgoing edges, the default edge is 
            // returned.
            return next;

        }


        //-Traversal--------------------------------------------------------//
        void Node::traverse(){
            if (is_active()){
                action_ptr->fire();
            }
        }


        //-Stream-representation--------------------------------------------//
        // friend function
        std::ostream &operator<<(std::ostream &os, Node const &node){
            return os << "<Node " << node.name << " at " << &node << ">";
        }

    }
}
