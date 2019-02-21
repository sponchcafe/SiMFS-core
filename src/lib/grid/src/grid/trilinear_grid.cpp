#include "grid/trilinear_grid.hpp"
#include <complex>

namespace sim{
    namespace grid{

        template <typename T>
        TrilinearGrid<T>::TrilinearGrid(GridSpace sp) : Grid<T>(sp){ }

        template <typename T>
        T TrilinearGrid<T>::get(Coordinate c){

            /* Trilinest inerpolation according to 
             * https://en.wikipedia.org/wiki/Trilinear_interpolation
             *
             */

            Coordinate nearest_point = Grid<T>::index_to_coordinate(Grid<T>::coordinate_to_index(c));

            auto delta = Grid<T>::delta;

            double fx = (c.x-nearest_point.x) / delta.x;
            double fy = (c.y-nearest_point.y) / delta.y;
            double fz = (c.z-nearest_point.z) / delta.z;

            T v000 = Grid<T>::get(c+Coordinate{0,       0,       0      });
            T v001 = Grid<T>::get(c+Coordinate{0,       0,       delta.z});
            T v010 = Grid<T>::get(c+Coordinate{0,       delta.y, 0      });
            T v011 = Grid<T>::get(c+Coordinate{0,       delta.y, delta.z});
            T v100 = Grid<T>::get(c+Coordinate{delta.x, 0,       0      });
            T v101 = Grid<T>::get(c+Coordinate{delta.x, 0,       delta.z});
            T v110 = Grid<T>::get(c+Coordinate{delta.x, delta.y, 0      });
            T v111 = Grid<T>::get(c+Coordinate{delta.x, delta.y, delta.z});

            T v00 = v000*(1-fx)+v100*fx;
            T v01 = v001*(1-fx)+v101*fx;
            T v10 = v010*(1-fx)+v110*fx;
            T v11 = v011*(1-fx)+v111*fx;

            T v0 = v00*(1-fy)+v10*fy;
            T v1 = v01*(1-fy)+v11*fy;

            T v = v0*(1-fz)+v1*fz;

            return v;

        }


        //-Explicit-template-instantiations----------------------------------//
        template class TrilinearGrid<unsigned>;
        template class TrilinearGrid<int>;
        template class TrilinearGrid<double>;
        template class TrilinearGrid<std::complex<double>>;
        //-------------------------------------------------------------------//

    }
}
