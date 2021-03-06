#pragma once

#include "focus/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Detection : public Focus{

            public:

                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                void set_json (json j) override;
                json get_json () override;
                void init () override;
                void run () override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_max_efficiency(double e);
                //-----------------------------------------------------------//
                 
                
            protected:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double max_efficiency = 1.0;
                //-----------------------------------------------------------//
                
        };
    }
}
