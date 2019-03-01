#pragma once

#include "grid/grid.hpp"
#include "shape.hpp"

namespace sim{
    namespace grid{

        template <typename T>
        class ShapeGridSerializer : public GridSerializer<T> {

            public:

                ShapeGridSerializer(std::iostream &ios, Grid<T> &g, focus::ShapeNorm &norm);
                void serialize() override;
                void deserialize() override;

            protected:

                void write_norm();
                void read_norm();

            private:

                sim::focus::ShapeNorm &norm;

        };
    }
}

