#include "serializer/shape_serializer.hpp"

namespace sim{
    namespace grid{

        template <typename T>
        ShapeGridSerializer<T>::ShapeGridSerializer(std::iostream &ios, Grid<T> &g, focus::ShapeNorm &norm) :
        GridSerializer<T>{ios, g}, norm(norm)
        {
        }

        template <typename T>
        void ShapeGridSerializer<T>::serialize(){
            write_norm();
            GridSerializer<T>::write_grid_space();
            GridSerializer<T>::write_grid_data();
        }

        template <typename T>
        void ShapeGridSerializer<T>::deserialize(){
            read_norm();
            GridSerializer<T>::read_grid_space();
            GridSerializer<T>::read_grid_data();
        }

        template <typename T>
        void ShapeGridSerializer<T>::write_norm(){
            GridSerializer<T>::write_binary(&norm, sizeof(norm));
        }

        template <typename T>
        void ShapeGridSerializer<T>::read_norm(){
            GridSerializer<T>::read_binary(&norm, sizeof(norm));
        }
        
        /*-Explcit-template-instantiation------------------------------------*/
        template class ShapeGridSerializer<double>;

    }
}

