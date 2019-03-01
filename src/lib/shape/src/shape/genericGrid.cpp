#include "shape/genericGrid.hpp"
#include "grid/nearest_grid.hpp"
#include "grid/trilinear_grid.hpp"
#include "serializer/shape_serializer.hpp"
#include <fstream>

namespace sim{
    namespace focus{

        //------------------------------------------------------------------//
        GenericGrid::GenericGrid(){ } 

        //------------------------------------------------------------------//
        void GenericGrid::set_interpolation_mode(InterpolationMode m) {
            mode = m;
        }

        void GenericGrid::set_grid_file (std::string fn) {
            fname = fn;
        }


        //------------------------------------------------------------------//
        void GenericGrid::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            auto m = params.at("interpolation").get<InterpolationMode>();
            set_interpolation_mode (m);
            set_grid_file(params.at("file"));

        }

        //------------------------------------------------------------------//
        json GenericGrid::get_json() {

            json j;

            j["file"] = fname;
            j["interpolation"] = mode;

            return j;

        }

        void GenericGrid::init() {
           
            // Determine grid type
            switch(mode){
                case InterpolationMode::NEAREST:
                    grid = std::make_unique<grid::NearestGrid<double>>();
                    break;
                case InterpolationMode::LINEAR:
                    grid = std::make_unique<grid::TrilinearGrid<double>>();
                    break;
                default:
                    std::cerr << "No such interpolation mode\n";
                    std::exit(1);
                    break;
            }
            
            // Load the grid
            std::fstream fs(fname, std::iostream::in | std::iostream::binary);
            if (!fs.good()) {
                std::cerr << "Error opening file\n";
                std::exit(1);
            }
            auto serializer = sim::grid::ShapeGridSerializer<double>(fs, *grid, norm);
            serializer.deserialize();
            fs.close();

            // initialize the function
        }

        //------------------------------------------------------------------//
        double GenericGrid::get_flux_density_prefactor () const {
            return norm.exi;
        }

        //------------------------------------------------------------------//
        double GenericGrid::get_efficiency_prefactor () const {
            return norm.det;
        }

        //------------------------------------------------------------------//
        double GenericGrid::evaluate(double x, double y, double z) const {
            sim::grid::Coordinate c{x,y,z};
            return prefactor * grid->get(c);
        }

    }

}
