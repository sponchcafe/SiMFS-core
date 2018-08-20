#pragma once 
#include <cmath>
#include "constants/constants.hpp"

namespace sim{

    namespace focus{

        class Focus{

            public:  

                //-----------------------------------------------------------//
                // Evaluates flux value > 0
                //-----------------------------------------------------------//
                virtual double evaluate(double x, double y, double z) const = 0;

        };

    }
}
