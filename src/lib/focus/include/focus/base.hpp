#ifndef SIM_FOCUS_BASE_H
#define SIM_FOCUS_BASE_H

/* Focus library of the FCS simulater package.
 * Till Zickmantel 18.1.2018
 * Draft for the library reorganisation.
 *
 * TODO:
 *   - Generic focus class
 *   - List of specific implementations extending the base class
 */

#include "definitions.hpp"

namespace sim{


    namespace focus{

        class BaseFocus{

            public:  

                virtual double evaluate(const sim::SI_Coordinate &c) const = 0;

            private:

        };

    }
}

#endif
