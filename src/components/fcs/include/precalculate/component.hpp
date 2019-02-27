#pragma once

#include "component/component.hpp"
#include "shape.hpp"
#include "grid/grid.hpp"

namespace sim{
    namespace comp{

        using namespace sim::focus;
        using namespace sim::grid;

        //-------------------------------------------------------------------//
        class Precalculator : public Component{

            public:

                //-----------------------------------------------------------//
                Precalculator ();

                //-----------------------------------------------------------//
                void set_filename(std::string fn);
                void set_shape_type(focus::ShapeType type);
                void set_focus_shape_ptr(std::unique_ptr<focus::FocusShape> &f);
                void set_grid_space(GridSpace gspace);

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//


            private:

                //-----------------------------------------------------------//
                std::string fname = "__focus__.dat";
                focus::ShapeType shape_type = focus::ShapeType::XYZ_GAUSS;
                std::unique_ptr<FocusShape> focus_shape_ptr;
                GridSpace grid_space{
                    LinSpace{-1e-6, 1e-6, 101},
                    LinSpace{-1e-6, 1e-6, 101},
                    LinSpace{-1e-6, 1e-6, 101}
                };
                focus::ShapeNorm norm{};
                Grid<double> grid{};

        };

    }
}
