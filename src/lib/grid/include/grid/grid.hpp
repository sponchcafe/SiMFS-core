#include "grid/coordinate.hpp" // Coordinate
#include <vector>
#include <functional>

namespace sim{
    namespace grid{

        //-------------------------------------------------------------------//
        template <typename T> class Grid{

            public:

                Grid(GridSpace sp);
                T get(Coordinate c);
                void set_nearest(Coordinate c, T value);
                void map(std::function<T(Coordinate&)> func);

            private:

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

        };

    }
}
