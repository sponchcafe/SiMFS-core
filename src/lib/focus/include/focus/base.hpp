#ifndef SIM_FOCUS_BASE_H
#define SIM_FOCUS_BASE_H

/* Focus library of the FCS simulater package.
 * Till Zickmantel 18.1.2018
 */

#include <cmath>
#include "json/json.hpp"

using json = nlohmann::json;

namespace sim{

    double const CONST_E = 2.7182818284590452353602874713527;
    double const CONST_C = 299792458.0;
    double const CONST_H = 6.62607004e-34;
    double const CONST_NA = 6.022140857e+23;
    double const CONST_PI = 3.141592653589793;

    namespace focus{

        class Focus{

            public:  

                //-----------------------------------------------------------//
                virtual double evaluate(double x, double y, double z) const = 0;

                //-----------------------------------------------------------//
                virtual void set_json(json j) = 0;

                //-----------------------------------------------------------//
                virtual json get_json() = 0;

            private:

        };

    }
}

#endif
