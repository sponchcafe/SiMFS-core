#pragma once

#include <type_traits>
#include <thread>
#include <iostream>
#include <json/json.hpp>
#include <initializer_list>
#include <functional>

#include "io/buffer.hpp"
#include "definitions/types.hpp"

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
        template <typename T> std::thread run_component(T &comp, bool init=false){
            std::thread thr{ [&comp, init] () { // copy init flag
                T _comp = std::move(comp); // stealing the component object
                if (init) {
                    _comp.init();
                }
                _comp.run(); 
            } };
            return thr;
        }

        using json_filter_t = std::function<bool(json const &)>;
    }
}

