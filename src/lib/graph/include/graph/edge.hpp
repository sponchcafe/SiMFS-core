#pragma once

#include "graph/definitions.hpp"
#include "graph/graph.hpp"
#include "graph/action.hpp"
#include "random/random.hpp"

#include <string>

namespace sim{
    namespace graph{

        //-Class:-Edge-------------------------------------------------------//
        // Represents and edge in the directed rate graph:
        //
        // - The edge has a unique name id and a refernce to its parent graph 
        // - The edge connects two nodes in the graph
        // - The edges target pointer refers to a node
        // - The *parent graph* has to guarantee the lifetime of this object
        // - The edge has an associated exponential distribution with rate
        //   constant lambda
        // - An edges lifetime is randomly generated from the underlying
        //   distribution
        // - Traversal of an edge can be linked to an action managed by the
        //   graph
        //-------------------------------------------------------------------//
        class Edge{

            public: 

                //-CONSTRUCTOR-----------------------------------------------//
                // graph:       parent graph reference
                // name:        unique id for the edge
                // source_name: name of the source node (outgoing)
                // target_name: name of the target node (incoming)
                // lambda:      rate constant for the lifetime calculation
                // seed:        random seed 
                //-----------------------------------------------------------//
                Edge(
                        Graph &graph,
                        std::string const name,
                        std::string const source_name, 
                        std::string const target_name, 
                        double lambda,
                        unsigned seed
                    );


                //-Access-target-node----------------------------------------//
                Node * get_target_node_ptr() const;


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
                void set_action(const std::string action_name);
                std::string const get_action_name() const;
                Action * get_action_ptr() const;
                bool is_active();


                //-Get-current-lifetime--------------------------------------//
                double get_lifetime() const;


                //-Reset-lambda----------------------------------------------//
                void set_lambda(const double lambda);
                

                //-Calculat-a-new-random-lifetime----------------------------//
                // Based on the edge's current lambda, a random lifetime is
                // generated from an exponential random distribution. The
                // lifetime value is returned and stored as the edge's new 
                // lifetime.
                //-----------------------------------------------------------//
                double calculate_lifetime();
                

                //-----------------------------------------------------------//
                void init();

    
                //-Simulation:-traversal-------------------------------------//
                // Traverse calls into the associated action's fire() method
                // unless action is nullptr.
                //-----------------------------------------------------------//
                void traverse();


                //-IDs-------------------------------------------------------//
                std::string const name;
                std::string const source_name;
                std::string const target_name;


                //-Stream-representation-------------------------------------//
                friend std::ostream &operator<<(
                        std::ostream &os, 
                        Edge const &edge);


            private:

                //-Parent-graph----------------------------------------------//
                Graph &graph;

                //-Current-lifetime------------------------------------------//
                double lifetime = 0.0;

                //-Target-node-pointer---------------------------------------//
                std::string target_node_name = EMPTY_NAME;
                Node *target_node_ptr = nullptr;

                //-Action-linked-to-edge-------------------------------------//
                std::string action_name = EMPTY_NAME;
                Action *action_ptr = nullptr;

                //-Random-distribution---------------------------------------//
                random::Exponential exponential;

        };

    }
}
