#pragma once 
#include <cmath>

namespace sim{

    /*
    double const CONST_E = 2.7182818284590452353602874713527;
    double const CONST_C = 299792458.0;
    double const CONST_H = 6.62607004e-34;
    double const CONST_NA = 6.022140857e+23;
    double const CONST_PI = 3.141592653589793;
    */

    namespace focus{

        class Focus{

            public:  

                //-----------------------------------------------------------//
                // Evaluates flux value > 0
                //-----------------------------------------------------------//
                virtual double evaluate(double x, double y, double z) const = 0;

            private:

        };

    }
}
