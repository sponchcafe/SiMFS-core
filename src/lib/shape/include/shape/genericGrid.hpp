#pragma once

#include "shape/base.hpp"
#include "json/json.hpp"
#include "grid/grid.hpp"

namespace sim{
    namespace focus{

        enum class InterpolationMode
        {
            NEAREST,
            LINEAR,
            NONE = -1
        };

        NLOHMANN_JSON_SERIALIZE_ENUM( InterpolationMode, {
            {InterpolationMode::NEAREST, "nearest"},
            {InterpolationMode::LINEAR, "linear"},
            {InterpolationMode::NONE, "__none__"}
        })

        //-------------------------------------------------------------------//
        class GenericGrid : public FocusShape{

            public:

                //-----------------------------------------------------------//
                GenericGrid();

                //-----------------------------------------------------------//
                void set_interpolation_mode(InterpolationMode m);
                void set_grid_file(std::string fn);

                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_density_prefactor() const override;
                double get_efficiency_prefactor() const override;
                
                //-----------------------------------------------------------//
                void init() override;

            private:

                //-----------------------------------------------------------//
                std::string fname = "__focus__.dat";
                InterpolationMode mode = InterpolationMode::NEAREST;
                ShapeNorm norm{1, 1};
                std::unique_ptr<grid::Grid<double>> grid; // Linear or Nearst Grid

        };

    }
}

