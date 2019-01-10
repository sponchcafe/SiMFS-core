#pragma once

#include "graph/graph.hpp"
#include "graph/definitions.hpp"
#include "json/json.hpp"
#include "definitions/types.hpp"

using json = nlohmann::json;

namespace sim{
    namespace graph{

        class Action{

            public:

                //-----------------------------------------------------------//
                static std::string type;

                //-----------------------------------------------------------//
                Action() = delete;

                //-----------------------------------------------------------//
                Action(Graph &graph, std::string const name);

                //-----------------------------------------------------------//
                // NOTE: A virtual destructor is necessary in an abstract
                // class to ensure the destructor of derived classes can be
                // invoked properly. In this case this is necessary to
                // trigger the stream flushing on destruction of actions 
                // owning a io stream.
                //-----------------------------------------------------------//
                virtual ~Action();

                //-----------------------------------------------------------//
                virtual void init();

                //-----------------------------------------------------------//
                virtual void fire();

                //-----------------------------------------------------------//
                std::ostream &print_info(std::ostream &os);

                //-----------------------------------------------------------//
                std::string const name;

            protected:

                //-----------------------------------------------------------//
                Graph &graph;

        };

        //-------------------------------------------------------------------//
        bool action_type_compare(std::string lhs, std::string rhs);

        //-------------------------------------------------------------------//
        template <class T, typename... Targs>
        std::unique_ptr<Action> make_action_uptr(Targs... Fargs){
            T *action_ptr = new T(Fargs...);
            return std::unique_ptr<Action>(action_ptr);
        }

    }
}

