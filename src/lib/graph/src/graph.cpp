#include "graph/graph.hpp"
#include "graph/edge.hpp"
#include "graph/node.hpp"
#include "graph/event.hpp"

#include <iostream>

namespace sim{
    namespace graph{

        //-------------------------------------------------------------------//
        //-CONSTRUCTOR-------------------------------------------------------//
        //-------------------------------------------------------------------//
        Graph::Graph(unsigned seed) : seed(seed) {

            // default node
            default_node = std::unique_ptr<Node>{
                new Node(*this, DEFAULT_NODE_NAME)
            };
            
            // default edge
            default_edge = std::unique_ptr<Edge>{
                new Edge(
                        *this,
                        DEFAULT_EDGE_NAME, // edge name
                        DEFAULT_NODE_NAME, // source name
                        DEFAULT_NODE_NAME, // target name
                        0, // lambda
                        0 // seed
                        )
            };

            // default action
            default_action = std::unique_ptr<Action>{
                new Action(*this, DEFAULT_ACTION_NAME) // TODO: Action factory
            };

            // link default loop
            default_node->add_edge(DEFAULT_EDGE_NAME);
            default_node->set_action(DEFAULT_ACTION_NAME);
            default_edge->set_action(DEFAULT_ACTION_NAME);

            // !!! MAYBE DANGEROUS IN CTOR ? //
            default_node->init();
            default_edge->init();
            default_action->init();

        }


        /*---Representation---------------*/
        /*void Graph::print_state(){
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
        }*/


        //-------------------------------------------------------------------//
        void Graph::add_node(std::string const name){
            if(!node_exists(name)) nodes.emplace_back(*this, name);
        }


        //-------------------------------------------------------------------//
        std::vector<Node> const & Graph::get_nodes() const {
            return nodes;
        }


        //-------------------------------------------------------------------//
        Node * Graph::get_node_ptr(std::string const name) {
            for (auto &node: nodes) if (node.name == name) return &node;
            return get_default_node_ptr();
        }


        //-------------------------------------------------------------------//
        Node * Graph::get_default_node_ptr() {
            return default_node.get();
        }


        //-------------------------------------------------------------------//
        size_t Graph::get_node_count() const{
            return nodes.size();
        }


        //-------------------------------------------------------------------//
        void Graph::add_edge(
                std::string const name,
                std::string const source_name,
                std::string const target_name,
                double lambda){

            if(!edge_exists(name)){
                edges.emplace_back
                    (
                        *this, // graph reference
                        name,
                        source_name,
                        target_name,
                        lambda,
                        get_new_seed()
                    );
            }

            // Register name with source node
            Node * source_node = get_node_ptr(source_name);
            source_node->add_edge(name);

        }


        //-------------------------------------------------------------------//
        std::vector<Edge> const & Graph::get_edges() const{
            return edges;
        }


        //-------------------------------------------------------------------//
        Edge * Graph::get_edge_ptr(std::string const name) {
            for (auto &it: edges) if (it.name == name) return &it;
            return get_default_edge_ptr();
        }


        //-------------------------------------------------------------------//
        Edge * Graph::get_default_edge_ptr() {
            return default_edge.get();
        }


        //-------------------------------------------------------------------//
        size_t Graph::get_edge_count() const {
            return edges.size();
        }


        //-------------------------------------------------------------------//
        void Graph::add_action(std::unique_ptr<Action> &action_uptr){
            if(!action_exists(action_uptr->name)){
                actions.push_back(std::move(action_uptr)); 
            }
        }
        
        
        //-------------------------------------------------------------------//
        std::vector<std::unique_ptr<Action>> const & Graph::get_actions() 
            const {
            return actions;
        }


        //-------------------------------------------------------------------//
        Action * Graph::get_action_ptr(std::string const name) {
            for (auto &it: actions) {
                if (it.get()->name == name) return it.get();
            }
            return get_default_action_ptr();
        }


        //-------------------------------------------------------------------//
        Action * Graph::get_default_action_ptr(){
            return default_action.get();
        }
        
        
        //-------------------------------------------------------------------//
        size_t Graph::get_action_count() const {
            return actions.size();
        }


        //-------------------------------------------------------------------//
        void Graph::link_node_action(
                std::string const node_name,
                std::string const action_name){
            Node * node = get_node_ptr(node_name);
            node->set_action(action_name);
        }

        
        //-------------------------------------------------------------------//
        void Graph::link_edge_action(
                std::string const edge_name,
                std::string const action_name){
            Edge * edge_ptr = get_edge_ptr(edge_name);
            edge_ptr->set_action(action_name);
        }


        //-------------------------------------------------------------------//
        void Graph::init(){

            for (auto &node: nodes) node.init();
            for (auto &edge: edges) edge.init();
            for (auto &action: actions) action->init();
            
        }

        
        //-------------------------------------------------------------------//
        void Graph::traverse() {

            Edge * next_path = get_default_edge_ptr();

            // Simulation mainloop
            while (!done){

                current->traverse(); 
                next_path = current->get_next();

                while(
                        !events.empty() 
                        && 
                        clock + next_path->get_lifetime() >= events.top().time
                     )
                {
                    execute_next_event();
                    if (done) return;
                    next_path = current->get_next(); // new try for lifetime
                }

                // path found, traversal ok
                clock += next_path->get_lifetime();
                next_path->traverse();
                current = next_path->get_target_node_ptr();

            }
                
        }

        
        //-------------------------------------------------------------------//
        void Graph::push_event(Event e){
            events.push(e);
        }


        //-------------------------------------------------------------------//
        void Graph::execute_next_event(){
            Event const &e = events.top();
            clock = e.time; // fast forward to event
            e.target->fire(); // executing event
            events.pop(); // remove event
        }


        //-------------------------------------------------------------------//
        size_t Graph::get_event_count() const{
            return events.size();
        }


        //-------------------------------------------------------------------//
        void Graph::set_clock(double time){
            clock = time;
        }


        //-------------------------------------------------------------------//
        void Graph::set_current(Node * node){
            current = node;
        }


        //-------------------------------------------------------------------//
        void Graph::set_done(bool state){
            done = state;
        }


        //-------------------------------------------------------------------//
        Node * Graph::get_current_ptr() const{
            return current;
        }


        //-------------------------------------------------------------------//
        double Graph::get_clock() const{
            return clock;
        }


        //-------------------------------------------------------------------//
        bool Graph::is_done() const{
            return done;
        }

        
        //-------------------------------------------------------------------//
        //-PRIVATE-----------------------------------------------------------//
        //-------------------------------------------------------------------//
        unsigned Graph::get_new_seed() const{
            return seed+1;
        }


        /*void Graph::print_actions(size_t indent) const {
            std::string ind(indent, ' ');
            for (auto it=actions.begin(); it!= actions.end(); ++it){
                Action *a = it->get();
                a->print_info(std::cerr);
                std::cerr << std::endl;
            }
        }

        void Graph::print_events(size_t indent) const {
            std::string ind(indent, ' ');
            std::cerr << ind << "Clock: " << clock << std::endl;
            std::cerr << ind << "Done: " << ((done) ? "yes" : "no") << std::endl;
            if (!events.empty()){
                Event e = events.top();
                std::cerr << ind << "Next event: " << e << std::endl;
            }
            std::cerr << ind << "Total pending: " << events.size() << std::endl;
        }

        void Graph::print_nodes(size_t indent) const {
            std::string ind(indent, ' ');
            for (auto it=nodes.begin(); it!= nodes.end(); ++it){
                std::cerr << std::endl;
                std::cerr << *it << std::endl;
                it->print_edges(std::cerr);
            }
        } */ 


        //-------------------------------------------------------------------//
        bool Graph::node_exists(std::string const name) const{
            for (auto &node: nodes) if (node.name == name) return true;
            return false;
        }
        

        //-------------------------------------------------------------------//
        bool Graph::edge_exists(std::string const name) const{
            for (auto &edge: edges) if (edge.name == name) return true;
            return false;
        }


        //-------------------------------------------------------------------//
        bool Graph::action_exists(std::string const name) const{
            for (auto &action: actions) if (action->name == name) return true;
            return false;
        }


    }
}

