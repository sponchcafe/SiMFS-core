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

    using json = nlohmann::json;

    //-----------------------------------------------------------------------//
    template <
        template<class InputT> class InputT,
        template<class OutputT> class OutputT
    > class Component{

        public: 

            //---------------------------------------------------------------//
            Component<InputT, OutputT>(){

                static_assert(
                    std::is_base_of<Input<double>, 
                    InputT<double>>::value,
                    "InputT has to be derived from the baseclass Input."
                );

                static_assert(
                    std::is_base_of<Output<double>, 
                    OutputT<double>>::value,
                    "OutputT has to be derived from the baseclass Output."
                );

            }

            //---------------------------------------------------------------//
            virtual void run() = 0;

            //---------------------------------------------------------------//
            virtual void configure_json(json j) {}

    };

}

#endif

