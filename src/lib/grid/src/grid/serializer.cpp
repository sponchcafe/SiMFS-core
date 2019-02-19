#include "grid/serializer.hpp"
#include <complex>

namespace sim{
    namespace grid{

        template <typename T>
        GridSerializer<T>::GridSerializer(std::iostream &ios, Grid<T> &g) :
            BaseIO(ios),
            grid{g}{
        }

        template <typename T>
        void GridSerializer<T>::serialize(){
            write_grid_space();
            write_grid_data();
        }

        template <typename T>
        void GridSerializer<T>::deserialize(){
            read_grid_space();
            read_grid_data();
        }

        template <typename T>
        void GridSerializer<T>::write_grid_space(){
            write_binary(&grid.grid_space, sizeof(grid.grid_space));
        }

        template <typename T>
        void GridSerializer<T>::write_grid_data(){
            write_binary(grid.get_buffer(), grid.data.size()*sizeof(T));
        }

        template <typename T>
        void GridSerializer<T>::read_grid_space(){
            read_binary(&grid.grid_space, sizeof(grid.grid_space));
        }

        template <typename T>
        void GridSerializer<T>::read_grid_data(){
            read_binary(grid.get_buffer(), grid.data.size()*sizeof(T));
        }

        template class GridSerializer<unsigned>;
        template class GridSerializer<int>;
        template class GridSerializer<double>;
        template class GridSerializer<std::complex<double>>;
        
    }
}
