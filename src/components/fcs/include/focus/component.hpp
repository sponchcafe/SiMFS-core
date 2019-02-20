#pragma once

#include "component/component.hpp"
#include "shape.hpp"
#include <map>


namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Focus : public Component{

            public:

                //-----------------------------------------------------------//
                Focus ();

                //-----------------------------------------------------------//
                virtual void set_json (json j) override;
                virtual json get_json () override;
                virtual void init () override;
                virtual void run () override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_shape_type(focus::ShapeType type);
                void set_output_id(std::string id);
                void set_input_id(std::string id);
                void set_focus_shape_ptr(std::unique_ptr<focus::FocusShape> &f);
                //-----------------------------------------------------------//
                 
                
            protected:

                //-----------------------------------------------------------//
                focus::ShapeType shape_type = focus::ShapeType::XYZ_GAUSS;
                
                std::string input_id = "__coordinates__";
                std::string output_id = "__focus__";
                
                //-----------------------------------------------------------//
                std::unique_ptr<focus::FocusShape> focus_shape_ptr;
                std::unique_ptr<io::BufferInput<Coordinate>> input_ptr;
                std::unique_ptr<io::BufferOutput<TimedValue>> output_ptr;

        };
    }
}
