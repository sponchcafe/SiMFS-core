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

#include "sim/types.hpp" // includes the coordinate definition

namespace sim{
    namespace focus{

        class BaseFocus{

            public:  

                virtual double evaluate(const sim::io::SI_Coordinate &c) const = 0;

            private:

        };

    }
}

#endif
