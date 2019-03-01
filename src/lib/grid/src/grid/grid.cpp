#include "grid/grid.hpp"
#include <complex>
#include <iostream>
#include <iomanip>

namespace sim{
    namespace grid{

        template <typename T>
        Grid<T>::Grid(GridSpace sp){
            grid_space = sp;
            resize();
        }

        template <typename T>
        void Grid<T>::resize(){
            data.resize(grid_space.x.n * grid_space.y.n * grid_space.z.n);
            delta = get_delta(grid_space);
        }

        template <typename T>
        T *Grid<T>::get_buffer(){
            return data.data();
        }

        template <typename T>
        T Grid<T>::get(Coordinate c){
            size_t index = coordinate_to_index(c);
            return data[index];
        }

        template <typename T>
        void Grid<T>::set(Coordinate c, T value){
            size_t index = coordinate_to_index(c);
            data[index] = value;
        }

        template <typename T>
        void Grid<T>::set(Coordinate c, T value, std::function<T(T, T)> func){
            size_t index = coordinate_to_index(c);
            data[index] = func(data[index], value);
        }


        template <typename T>
        void Grid<T>::map(std::function<T(Coordinate&)> func){

            unsigned n_hardware_threads = std::thread::hardware_concurrency();
            if (n_hardware_threads < 1) n_hardware_threads = 1;
            std::cerr << "Precalculating (" << n_hardware_threads << " threads):";
            size_t section_size = data.size()/n_hardware_threads;

            std::atomic<size_t> progress{0};
            std::vector<std::thread> threads;

            for (unsigned i=0; i<n_hardware_threads; ++i){

                size_t start = i*section_size;
                size_t end = (i+1)*section_size;

                // last section until end of grid
                if (end > (n_hardware_threads-1)*section_size) end = data.size();

                threads.emplace_back([=, &progress] () { 
                        map_grid_section(start, end, func, std::ref(progress));
                });

            }

            // progress display
            threads.emplace_back([=, &progress] () { 
                    print_progress(data.size(), std::ref(progress));
                    });

            for (auto &it: threads) it.join();

        }


        template <typename T>
        void Grid<T>::print_progress(size_t n, std::atomic<size_t> &prg){
            size_t p = 0;
            while (p < n){
                std::cerr << std::setw(4) << p*100/n << " %\b\b\b\b\b\b";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                p = prg.load();
            }
            std::cerr << std::setw(4) << p*100/n << '\n';
        }

        template <typename T>
        Coordinate Grid<T>::get_delta(GridSpace sp){
            Coordinate delta{};
            delta.x = (sp.x.max - sp.x.min) / (sp.x.n - 1);
            delta.y = (sp.y.max - sp.y.min) / (sp.y.n - 1);
            delta.z = (sp.z.max - sp.z.min) / (sp.z.n - 1);
            return delta;
        }

        template <typename T>
        size_t Grid<T>::discretize_single_coord(LinSpace d, double c){
            int index = ((c-d.min)/(d.max-d.min))*(d.n-1);
            index = (index < 0) ? 0 : index;                // i<0   -> i:=0
            index = (index > (int) d.n-1) ? d.n-1 : index;  // i>max -> i:=max
            return index;
        }

        template <typename T>
        size_t Grid<T>::coordinate_to_index(Coordinate c){

            size_t index_x = discretize_single_coord(grid_space.x, c.x);
            size_t index_y = discretize_single_coord(grid_space.y, c.y);
            size_t index_z = discretize_single_coord(grid_space.z, c.z);

            size_t index = 
                  index_x * grid_space.y.n * grid_space.z.n 
                + index_y * grid_space.z.n 
                + index_z;

            return index;

        }

        template <typename T>
        Coordinate Grid<T>::index_to_coordinate(size_t index){

            size_t index_x = index / (grid_space.y.n*grid_space.z.n) % grid_space.x.n;
            size_t index_y = index /  grid_space.z.n                 % grid_space.y.n;
            size_t index_z = index                                   % grid_space.z.n;

            double x = index_x*delta.x + grid_space.x.min;
            double y = index_y*delta.y + grid_space.y.min;
            double z = index_z*delta.z + grid_space.z.min;

            Coordinate c {x, y, z};

            return c;

        }

        template <typename T>
        void Grid<T>::map_grid_section(size_t start, size_t end, 
            std::function<T(Coordinate &c)> func, std::atomic<size_t> &prog){

            for (size_t i=start; i<end; ++i){
                Coordinate c = index_to_coordinate(i);
                data[i] = func(c); 
                ++prog;
            }

        }

        //-Explicit-template-instantiations----------------------------------//
        template class Grid<unsigned>;
        template class Grid<int>;
        template class Grid<double>;
        template class Grid<std::complex<double>>;
        //-------------------------------------------------------------------//

    }
}
