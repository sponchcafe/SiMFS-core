#include "efield/storage.hpp"
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>

namespace sim{
    namespace field{

        //------------------------------------------------------------------//
        bool check_file_exists(std::string fname){
            auto fs = std::ifstream(fname, std::ifstream::binary);
            bool exists = fs.good();
            fs.close();
            return exists;
        }

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

            std::cerr << "Field written to " << fname << "\n";

        }

        //------------------------------------------------------------------//
        void make_grid_coordinates(EFieldSpec spec, std::vector<EFieldCoordinate> &t){

            double x_step = 0.0, y_step = 0.0, z_step = 0.0;
            if (spec.x.n > 1) x_step = (spec.x.max-spec.x.min) / (spec.x.n-1);
            if (spec.y.n > 1) y_step = (spec.y.max-spec.y.min) / (spec.y.n-1);
            if (spec.z.n > 1) z_step = (spec.z.max-spec.z.min) / (spec.z.n-1);

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
        void evaluate_prefactors(EField &f, EFieldSpec &spec){

            std::cerr << "Calculating prefactors\n";

            // Efficiency prefactor
            spec.efficiency_prefactor = 1/f.evaluate_intensity(0,0,0);

            // Flux density prefactor
            EFieldSpec z0_grid = spec;
            z0_grid.z = {0,0,1};
            
            std::vector<EFieldCoordinate> xy;
            make_grid_coordinates(z0_grid, xy);

            std::vector<EFieldComponents> field;
            double dx = (z0_grid.x.max-spec.x.min) / (z0_grid.x.n-1);
            double dy = (z0_grid.y.max-spec.y.min) / (z0_grid.y.n-1);
            double area = 0.0;

            for (auto &it: xy) area += f.evaluate_intensity(it.x, it.y, it.z);
            spec.flux_density_prefactor = 1/(area*dx*dy);
        }

        //------------------------------------------------------------------//
        void evaluate_efield_grid_section(
                EField &f, 
                std::atomic<size_t> &prg,
                std::vector<EFieldCoordinate>::iterator c_begin,
                std::vector<EFieldCoordinate>::iterator c_end,
                std::vector<EFieldComponents>::iterator f_begin)
        {
            auto c_current = c_begin;
            auto f_current = f_begin;
            while(c_current != c_end){
                *f_current = f.evaluate_field(c_current->x, c_current->y, c_current->z);
                ++prg;
                ++c_current;
                ++f_current;
            }
        }

            

        //------------------------------------------------------------------//
        void evaluate_efield_grid(EField &f, std::vector<EFieldCoordinate> &c, std::vector<EFieldComponents> &t){

            size_t n = c.size();
            t.resize(n);
            unsigned int n_threads = std::thread::hardware_concurrency();
            n_threads = (n_threads) ? n_threads : 1;
            std::cerr << "Calculating field (" << n_threads << " threads):";

            std::atomic<size_t> progress{0};
            std::vector<std::thread> threads;
            for (size_t i=0; i<n_threads; i++){

                auto c_begin = c.begin()+i*n/n_threads;
                auto t_begin = t.begin()+i*n/n_threads;
                auto c_end = c.begin()+(i+1)*n/n_threads;

                threads.emplace_back(
                        evaluate_efield_grid_section, 
                        std::ref(f), 
                        std::ref(progress),
                        c_begin, 
                        c_end, 
                        t_begin
                        );

            }

            threads.emplace_back(
                [&] () {
                    size_t p = 0;
                    while (p < n){
                        std::cerr << std::setw(4) << p*100/n << " %\b\b\b\b\b\b";
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        p = progress.load();
                    }
                    std::cerr << std::setw(4) << p*100/n << '\n';
                }
            );

            for (auto &t: threads) t.join();

        }

    }
}
