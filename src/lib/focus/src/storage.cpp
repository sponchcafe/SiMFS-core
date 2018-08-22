#include "efield/storage.hpp"
#include <fstream>

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

            for (size_t iz = 0; iz < spec.z.n; ++iz){
                for (size_t iy = 0; iy < spec.y.n; ++iy){
                    for (size_t ix = 0; ix < spec.x.n; ++ix){
                       t.push_back({ix*x_step, iy*y_step, iz*z_step}); 
                    }
                }
            }


        }

        //------------------------------------------------------------------//
        void evaluate_efield_grid(focus::EField &f, std::vector<EFieldCoordinate> &c, std::vector<EFieldComponents> &t){
            for (auto &ci: c){
                t.push_back(f.evaluate_field(ci.x, ci.y, ci.z));
            }
        }



    }
}
