#pragma once

#include "component/component.hpp"
#include "component/types.hpp"
#include "focus/base.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Detection : public Component{

            public:

                //-----------------------------------------------------------//
                Detection ();

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_origin_efficiency(double e);
                void set_coordinate_input_id(std::string id);
                void set_efficiency_output_id(std::string id);
                void set_focus(json params);
                //-----------------------------------------------------------//
                 
            private:

                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double origin_efficiency = 1.0;
                std::string coordinate_input_id = "./coords";
                std::string efficiency_output_id = "./detection_efficiency";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                double focus_scaling = 1.0;
                Coordinate c{0.0,0.0,0.0,0.0};
                TimedValue efficiency{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Focus> focus_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> coordinate_input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> efficiency_output_ptr;

        };
    }
}
