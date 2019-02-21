#pragma once
#include "grid.hpp"

namespace sim{
    namespace grid{

        template <typename T>
        class TrilinearGrid : public Grid<T>{

            public:

                TrilinearGrid(){};
                TrilinearGrid(GridSpace sp);
                T get(Coordinate c) override;

        };

    }
}
