#pragma once

#include "grid/grid.hpp"
#include "baseIO/baseIO.hpp"

namespace sim{
    namespace grid{

        template <typename T> class Grid; // forward declaration

        template <typename T>
        class GridSerializer : public io::BaseIO {

            public:

                GridSerializer(std::iostream &ios, Grid<T> &g);
                void serialize();
                void deserialize();

            private:

                void write_grid_space();
                void write_grid_data();
                void read_grid_space();
                void read_grid_data();

                Grid<T> &grid;

        };

    }
}
