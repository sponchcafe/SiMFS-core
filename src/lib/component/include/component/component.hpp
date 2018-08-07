#pragma once

#include <type_traits>
#include <thread>
#include <iostream>
#include <json/json.hpp>
#include <initializer_list>
#include <functional>

#include "io/buffer.hpp"

//---------------------------------------------------------------------------//
// Abstract base definition of the component interface.
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

namespace sim{
    namespace comp{

        using json = nlohmann::json;

        class Component{

            public: 

                //-----------------------------------------------------------//
                Component(){} 

                //-----------------------------------------------------------//
                virtual ~Component() {}

                //-----------------------------------------------------------//
                virtual void set_json(json j) = 0;

                //-----------------------------------------------------------//
                virtual json get_json() = 0;

                //-----------------------------------------------------------//
                virtual void run() = 0;

                //-----------------------------------------------------------//
                virtual void init() = 0;

        };

        //-Thread-helper-----------------------------------------------------//
        template <typename T> std::thread run_component(T &comp){
            std::thread thr{ [&] () { 
                T _comp = std::move(comp); // stealing the component object
                _comp.run(); 
            } };
            return thr;
        }

        //-JSON-filters------------------------------------------------------//
        using json_filter_t = std::function<bool(json const &)>;
        bool filter_json(json const &j, json_filter_t filter);
        bool filter_json_all(json const &j, std::initializer_list<json_filter_t> filters);
        bool filter_json_any(json const &j, std::initializer_list<json_filter_t> filters);

    }
}

