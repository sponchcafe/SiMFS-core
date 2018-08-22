#include "efield/storage.hpp"
#include <fstream>
#include <thread>

namespace sim{
    namespace field{

        //------------------------------------------------------------------//
        void load_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &t){

            auto fs = std::ifstream(fname, std::ifstream::binary);

            if (!fs.good()){
                fs.close();
                std::cerr << "Failed to open field file: " << fname << "\n";
                return;
            }

            fs.read(reinterpret_cast<char *>(&header), sizeof(EFieldSpec));
            if(fs.gcount() != sizeof(EFieldSpec)){
                std::cerr << "Error reading field file " << fname << " header.\n";
            }

            size_t field_size = header.x.n * header.y.n * header.z.n;
            size_t n_bytes = field_size * sizeof(EFieldComponents);

            t.resize(field_size);
            fs.read(reinterpret_cast<char *>(t.data()), n_bytes);
            
            if (fs.gcount() != n_bytes) {
                std::cerr << "Error reading field file " << fname << " body.\n";
            }

            fs.close();
            
        }

        //------------------------------------------------------------------//
        void save_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &s){

            auto ifs = std::ifstream(fname, std::ifstream::binary);
            if (ifs.good()){
                std::cerr << "Field file " << fname << " already exists.\n";
                ifs.close();
                return;
            } 

            auto fs = std::ofstream(fname, std::ofstream::binary);
            if (!fs.good()){
                fs.close();
                std::cerr << "Failed to open field file: " << fname << "\n";
                return;
            }

            fs.write(reinterpret_cast<char *>(&header), sizeof(EFieldSpec));
            fs.write(reinterpret_cast<char *>(s.data()), s.size()*sizeof(EFieldComponents));

            fs.close();

        }

        //------------------------------------------------------------------//
        void make_grid_coordinates(EFieldSpec spec, std::vector<EFieldCoordinate> &t){

            double x_step = (spec.x.max-spec.x.min) / spec.x.n;
            double y_step = (spec.y.max-spec.y.min) / spec.y.n;
            double z_step = (spec.z.max-spec.z.min) / spec.z.n;

            for (size_t ix = 0; ix < spec.x.n; ++ix){
                for (size_t iy = 0; iy < spec.y.n; ++iy){
                    for (size_t iz = 0; iz < spec.z.n; ++iz){
                        double x = spec.x.min + ix*x_step;
                        double y = spec.y.min + iy*y_step;
                        double z = spec.z.min + iz*z_step;
                        t.push_back({x, y, z});
                    }
                }
            }


        }

        //------------------------------------------------------------------//
        void evaluate_efield_grid_section(EField &f, 
                std::vector<EFieldCoordinate>::iterator c_begin,
                std::vector<EFieldCoordinate>::iterator c_end,
                std::vector<EFieldComponents>::iterator f_begin){
            auto c_current = c_begin;
            auto f_current = f_begin;
            while(c_current != c_end){
                *f_current = f.evaluate_field(c_current->x, c_current->y, c_current->z);
                ++c_current;
                ++f_current;
            }
        }

            

        //------------------------------------------------------------------//
        void evaluate_efield_grid(EField &f, std::vector<EFieldCoordinate> &c, std::vector<EFieldComponents> &t){
            size_t n = c.size();
            t.resize(n);
            size_t count = 0;
            int print_percentage = 0;
            unsigned int n_threads = std::thread::hardware_concurrency();
            std::cerr << "Working with " << n_threads << " threads\n";

            std::vector<std::thread> threads;
            for (size_t i=0; i<n_threads; i++){

                auto c_begin = c.begin()+i*n/n_threads;
                auto t_begin = t.begin()+i*n/n_threads;
                auto c_end = c.begin()+(i+1)*n/n_threads;

                threads.emplace_back(evaluate_efield_grid_section, std::ref(f), c_begin, c_end, t_begin);

            }

            for (auto &t: threads) t.join();

        }

    }
}
