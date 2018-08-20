#pragma once 
#include <cmath>
#include "definitions/constants.hpp"
#include "definitions/types.hpp"

namespace sim{

    //-----------------------------------------------------------------------//
    namespace focus{

        //-------------------------------------------------------------------//
        class Focus{

            public:  

                //-----------------------------------------------------------//
                virtual double evaluate(double x, double y, double z) const = 0;
                void set_prefactor(double p) { prefactor = p; }


            protected:

                double prefactor = 1.0;

        };

    }
}
