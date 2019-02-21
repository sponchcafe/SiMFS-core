#include "grid/nearest_grid.hpp"
#include <complex>

namespace sim{
    namespace grid{

        template <typename T>
        NearestGrid<T>::NearestGrid(GridSpace sp) : Grid<T>(sp){ }

        template <typename T>
        T NearestGrid<T>::get(Coordinate c){
            size_t index = Grid<T>::coordinate_to_index(c+Grid<T>::delta*0.5);
            return Grid<T>::data[index];
        }

        template <typename T>
        void NearestGrid<T>::set(Coordinate c, T value){
            size_t index = Grid<T>::coordinate_to_index(c+Grid<T>::delta*0.5);
            Grid<T>::data[index] = value;
        }


        //-Explicit-template-instantiations----------------------------------//
        template class NearestGrid<unsigned>;
        template class NearestGrid<int>;
        template class NearestGrid<double>;
        template class NearestGrid<std::complex<double>>;
        //-------------------------------------------------------------------//

    }
}
