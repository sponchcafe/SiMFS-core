#pragma once

#include "grid/coordinate.hpp" // Coordinate
#include "grid/serializer.hpp"
#include <vector>
#include <functional>

namespace sim{
    namespace grid{

        //-------------------------------------------------------------------//
        template <typename T> class Grid{

            public:

                Grid(){};
                Grid(GridSpace sp);
                virtual T get(Coordinate c);
                virtual void set(Coordinate c, T value);
                virtual void set(Coordinate c, T value, std::function<T(T, T)> func);
                void map(std::function<T(Coordinate&)> func);
                T* get_buffer();

            protected:

                //-----------------------------------------------------------//
                size_t coordinate_to_index(Coordinate c);
                size_t discretize_single_coord(LinSpace d, double c);

                //-----------------------------------------------------------//
                Coordinate index_to_coordinate(size_t index);
                Coordinate get_delta(GridSpace spec);

                void map_grid_section(size_t start, size_t end, 
                        std::function<T(Coordinate&)> func);

                GridSpace grid_space;
                Coordinate delta;
                std::vector<T> data;

            private:

                template<typename>
                friend class GridSerializer;

        };

    }
}
