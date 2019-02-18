#pragma once
#include "grid.hpp"

namespace sim{
    namespace grid{

        template <typename T>
        class NearestGrid : public Grid<T>{

            public:

                NearestGrid(GridSpace sp);
                T get(Coordinate c) override;
                void set(Coordinate c, T value) override;

        };

    }
}
