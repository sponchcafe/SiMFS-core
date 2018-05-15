#ifndef SIM_BASE_COMPONENT_H
#define SIM_BASE_COMPONENT_H

#include <type_traits>
#include <iostream>
#include <json/json.hpp>

#include "io/base_io.hpp"

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
    }

}

#endif

