#pragma once

#include "graph/definitions.hpp"

#include <string>
#include <vector>
#include <queue>

namespace sim{
    namespace graph{

        /* Forward declarations */
        class Node;
        class Edge;
        class Action;
        class Event;
        class Graph;

        //-Class:-Graph------------------------------------------------------//
        // Represents a directed multi-linked graph with average transition
        // rates over all edges.
        //
        // - The graph owns and manages all node, edge and action objects
        // - It guarantees their lifetime and memory location for the time of
        //   the simulation (traversal through the graph)
        // - Nodes, edges and actions are identified by a unique name
        // - At runtime, connections are resolved from the specified named
        //   relations to constant (non-owning) pointers
        //-------------------------------------------------------------------//
        class Graph{

            public:

                //-CONSTRUCTOR-----------------------------------------------//
                Graph(unsigned seed);

                //-Setup:-Adding-nodes---------------------------------------//
                // name:    id of the new node
                // Repeated additions of the same name are ignored
                //-----------------------------------------------------------//
                void add_node(std::string const name);
                std::vector<Node> const & get_nodes() const;
                Node * get_node_ptr(std::string const name);
                Node * get_default_node_ptr();
                size_t get_node_count() const;



                //-Setup:-Adding-edge----------------------------------------//
                // name:    id of the new edge
                // Repeated additions of the same name are ignored
                //-----------------------------------------------------------//
                void add_edge(
                        std::string const name, 
                        std::string const source, 
                        std::string const target, 
                        double lambda = 0);
                std::vector<Edge> const & get_edges() const;
                Edge * get_edge_ptr(std::string const name);
                Edge * get_default_edge_ptr();
                size_t get_edge_count() const;


                //-Setup:-Adding-action--------------------------------------//
                // action:  pointer to a newly created action instance (i.e.
                //          a derived class instance)
                // The graph object takes ownership of the action instance
                // via a unique_ptr and manages its lifetime from hereon.
                //-----------------------------------------------------------//
                void add_action(std::unique_ptr<Action> &action_uptr);
                // Convenience variadic action adding
                template <class T, typename... Targs>
                void add_action(Targs... Fargs){
                    T *action_ptr = new T(Fargs...);
                    auto uptr = std::unique_ptr<Action>(action_ptr);
                    add_action(uptr);
                }

                std::vector<std::unique_ptr<Action>> const & get_actions() const;
                Action * get_action_ptr(std::string const name);
                Action * get_default_action_ptr();
                size_t get_action_count() const;

                //-Link-action-to-node---------------------------------------//
                // node_name:   id of the node to link an action to
                // action_name: id of the action to be linked
                // - if node_name is not valid, the operation is ignored, the
                //   graph remains unchanged
                // - if action_name is not valid, the graphs default action 
                //   will be linked insted, leading to an error message at 
                //   runtime
                //-----------------------------------------------------------//
                void link_node_action(
                        std::string const node_name, 
                        std::string const action_name);


                //-Link-action-to-edge---------------------------------------//
                // edge_name:   id of the edge to link an action to
                // action_name: id of the action to be linked
                // - if edge_name is not valid, the operation is ignored, the
                //   graph remains unchanged
                // - if action_name is not valid, the graphs default action 
                //   will be linked insted, leading to an error message at 
                //   runtime
                //-----------------------------------------------------------//
                void link_edge_action(
                        std::string const edge_name,
                        std::string const action_name);


                //-Debugging-output------------------------------------------//
                void print_state(); // TODO: make prettier....
                void print_actions(size_t indent) const;
                void print_events(size_t indent) const;
                void print_nodes(size_t indent) const;


                //-Simulation:-initialization--------------------------------//
                // start_node_name: id of the node to start the simulation at
                // - init performs 3 tasks:
                //      1. Resolves the graph connections from string ids
                //      2. Resolves the actions from their string ids
                //      3. Calls the init method of all actions
                // - *NOTE:* init has to always to be called befor starting
                //   graph traversal and again when the graph structure is
                //   changed. Otherwise behaviour is undefined.
                //-----------------------------------------------------------//
                void init(); 


                //-Simulation:-Mainloop-for-graph-traversal------------------//
                // Runs the traversal of the directed graphs starting at the
                // current node. The function runs indefinetily until the done
                // flag is set by some action-call.
                //-----------------------------------------------------------//
                void traverse();


                //-Simulation:-Issue-event-----------------------------------//
                // event:   instance of event to be executed at its fire-time
                //-----------------------------------------------------------//
                void push_event(Event event);
                size_t get_event_count() const;


                //-Set-state-------------------------------------------------//
                void set_clock(double time);
                void set_current(Node * node_ptr);
                void set_done(bool state);


                //-Get-state-------------------------------------------------//
                Node * get_current_ptr() const;
                double get_clock() const;
                bool is_done() const;

                //-Get-new-seed-based-on-the-graph-seed----------------------//
                unsigned get_new_seed();

            private:

                void init_defaults();

                //-Check-existence-------------------------------------------//
                bool node_exists(std::string const name) const;
                bool edge_exists(std::string const name) const;
                bool action_exists(std::string const name) const;


                //-Execute-event---------------------------------------------//
                // The next event in the queue will be executed. Its target
                // action's fire method is called once after the graph clock
                // is set to the event's time
                //-----------------------------------------------------------//
                void execute_next_event();


                //-Strcutre--------------------------------------------------//
                std::unique_ptr<Node> default_node;
                std::unique_ptr<Edge> default_edge;
                std::unique_ptr<Action> default_action;

                std::vector<Node> nodes;
                std::vector<Edge> edges;
                std::vector<std::unique_ptr<Action>> actions;


                //-Random-seed-----------------------------------------------//
                unsigned seed;


                //-State-----------------------------------------------------//
                double clock = 0.0;                 // real time             //
                bool done = false;                  // termination flag      // 
                Node *current = nullptr;            // current node          //
                std::priority_queue<Event> events;  // event queue           //
                //-----------------------------------------------------------//

        };

    }
}
