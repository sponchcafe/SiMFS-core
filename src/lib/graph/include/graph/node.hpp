#ifndef SIM_GRAPH_NODE_H
#define SIM_GRAPH_NODE_H

#include "graph/definitions.hpp"
#include "graph/graph.hpp"

#include <string>
#include <vector>
#include <iostream>

namespace sim{
    namespace graph{
        
        //-Class:-Node-------------------------------------------------------//
        // Represents a node in the directed rate graph:
        //
        // - The node has a unique name id and a refernce to its parent graph 
        // - The node holds a list of non-owning pointers to its outgoing edges 
        // - The *parent graph* has to guarantee the lifetime of theses objects
        // - Traversal of a node can be linked to an action managed by the
        //   graph
        //-------------------------------------------------------------------//
        class Node{

            public:

                //-CONSTRUCTOR-----------------------------------------------//
                // graph:   parent graph reference
                // name:    unique id for the node
                //-----------------------------------------------------------//
                Node(Graph &graph, std::string const name);


                //-Setup:-Outgoing-edges-------------------------------------//
                // edge_name:   ID of the edge to add to the node
                //
                // The edge_name is resolved to a pointer by the init method
                // at runtime. If edge_name is not found, the default edge 
                // will be added.
                //
                // Edges can be retrieved as string IDs or resolved pointers
                // - get_edge_names:    get vector of edged names
                // - get_edges:         get vector of resolved edge pointers 
                //-----------------------------------------------------------//
                void add_edge(const std::string edge_name);
                std::vector<std::string const> const &get_edge_names();
                std::vector<Edge *> const &get_edges();
                    

                //-Setup:-Linked-action--------------------------------------//
                // action_name: ID of the action to be linked 
                //
                // The action_name is resolved to a pointer by the init method
                // at runtime. If action_name is not found, the default action
                // will be linked.
                //
                // Action can be retrieved as a string ID or resolved pointer
                // - get_action_name:   get action name
                // - get_action_ptr:    get resolved action pointer
                //-----------------------------------------------------------//
                void set_action(std::string const action_name);
                std::string const get_action_name(); 
                Action * get_action_ptr();
                bool is_active();

            
                //-Initialization--------------------------------------------//
                // Resolution of names to pointers: Each edge name is resolved
                // to an edge pointer, the action name is resolved to an action
                // pointer. 
                //-----------------------------------------------------------//
                void init();


                //-Traversal-of-node-----------------------------------------//
                // Traverse calls into the associated action's fire() method
                // unless action is nullptr.
                //-----------------------------------------------------------//
                void traverse();


                //-Get-the-next-path-to-traverse-----------------------------//
                // returns a pointer to the edge with the shortest lifetime
                // calculated according to each edge's get_lifetime method 
                // if node has not outgoing edges, the grapgs default_edge is
                // returned
                //-----------------------------------------------------------//
                Edge * get_next();


                //-Stream-representation-------------------------------------//
                friend std::ostream &operator<<(
                        std::ostream &os, 
                        Node const &node);


                //-The-nodes-unique-name-------------------------------------//
                std::string const name;

            private:

                //-Parent-graph----------------------------------------------//
                Graph &graph; 

                //-Outgoing-edges--------------------------------------------//
                std::vector<std::string const> outgoing_edge_names;
                std::vector<Edge *> outgoing_edge_ptrs;

                //-Action-linked-to-node-------------------------------------//
                std::string action_name = EMPTY_NAME;
                Action *action_ptr = nullptr;

        };

    }
}

#endif
