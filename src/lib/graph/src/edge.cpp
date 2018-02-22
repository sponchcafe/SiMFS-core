#include "graph/edge.hpp"

#include <limits>

namespace sim{
    namespace graph{

        //-CONSTRUCTOR-------------------------------------------------------//
        Edge::Edge(
                Graph &graph,
                std::string const name, 
                std::string const source_name,
                std::string const target_name,
                double lambda,
                unsigned seed) :
            name(name),
            source_name(source_name),
            target_name(target_name),
            graph(graph),
            lifetime(std::numeric_limits<double>::max()), // Init max for proper lambda=0 behaviour
            exponential(random::Exponential(lambda, seed))
        {}

        
        //-------------------------------------------------------------------//
        Node * Edge::get_target_node_ptr() const{
            return target_node_ptr;
        }
        
        //-Setup:-Link-an-action---------------------------------------------//
        void Edge::set_action(std::string const action_name){
            this->action_name = action_name;
        }


        //-Acces-linkes-action-----------------------------------------------//
        std::string const Edge::get_action_name() const{
            return action_name;
        }

        
        //-------------------------------------------------------------------//
        Action * Edge::get_action_ptr() const{
            if (action_ptr == nullptr) return graph.get_default_action_ptr();
            return action_ptr;
        }


        //-------------------------------------------------------------------//
        bool Edge::is_active() {
            return action_ptr != nullptr;
        }


        //-Accese-current-lifetime-------------------------------------------//
        double Edge::get_lifetime() const{
            return lifetime;
        }


        //-Reset-lambda------------------------------------------------------//
        void Edge::set_lambda(double lambda){
            exponential.set_lambda(lambda);
        }


        //-Calculat-a-new-random-lifetime------------------------------------//
        double Edge::calculate_lifetime(){
            lifetime = exponential();
            return lifetime;
        }


        //-------------------------------------------------------------------//
        void Edge::init(){

            // init action, EMPTY_NAME -> action = nullptr (no action)
            if (action_name != EMPTY_NAME){
                action_ptr = graph.get_action_ptr(action_name);
            }

            // init traget pointer
            target_node_ptr = graph.get_node_ptr(target_name);

        }
        

        //-Simulation:-traversal---------------------------------------------//
        void Edge::traverse(){
            if (is_active()){
                action_ptr->fire();
            }
        }

        //-Stream-representation---------------------------------------------//
        std::ostream &operator<<(std::ostream &os, const Edge &edge){
            os << "<Edge " << edge.name << " at " << &edge;
            os << ", " << edge.source_name << " -> " << edge.target_name;
            os << ", lambda=" << edge.exponential.get_lambda();
            os << ", lifetime=" << edge.lifetime << ">";
            return os;
        }
    }
}
