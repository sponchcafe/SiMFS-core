#include "precalculate/component.hpp"
#include "serializer/shape_serializer.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Precalculator::Precalculator() {}

        //-------------------------------------------------------------------//
        void Precalculator::set_filename(std::string fn){
            fname = fn;
        }
        void Precalculator::set_shape_type(focus::ShapeType type){
            shape_type = type;
        }
        void Precalculator::set_focus_shape_ptr(std::unique_ptr<focus::FocusShape> &f){
            focus_shape_ptr = std::move(f);
        }
        void Precalculator::set_grid_space(GridSpace gspace){
            grid_space = gspace;
        }

        //-------------------------------------------------------------------//
        void Precalculator::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_shape_type(params.at("type").get<focus::ShapeType>());
            set_filename(params.at("filename"));

            GridSpace gs;
            from_json(params.at("grid"), gs);
            set_grid_space(gs);

            json shape_params = params["shape"];
            auto shape = focus_shape_factory(shape_type, shape_params);
            set_focus_shape_ptr(shape);


        }

        //-------------------------------------------------------------------//
        json Precalculator::get_json(){

            json j;

            j["filename"] = fname;
            j["type"] = shape_type;

            to_json(j["grid"], grid_space);

            if (focus_shape_ptr) {
                j["shape"] = focus_shape_ptr->get_json();
            } else{
                j["shape"] = json::object();
            }
            
            return j;

        }

        //-------------------------------------------------------------------//
        void Precalculator::init() {
            focus_shape_ptr->init();
            grid = Grid<double>(grid_space);
            norm.det = focus_shape_ptr->get_efficiency_prefactor();
            norm.exi = focus_shape_ptr->get_flux_density_prefactor();
        }
            

        //-------------------------------------------------------------------//
        void Precalculator::run(){
            grid.map([=] (sim::grid::Coordinate &c) -> double{
                    return focus_shape_ptr->evaluate(c.x, c.y, c.z);
                    });

            std::fstream fs(fname, std::fstream::out | std::iostream::binary);
            if (!fs.good()){
                std::cerr << "Error opening file\n";
                std::exit(1);
            }

            auto serializer = ShapeGridSerializer<double>(fs, grid, norm);
            serializer.serialize();
            fs.close();

        }

        /*
        //-------------------------------------------------------------------//
        void to_json(json& j, const LinSpace& l){
            j = json{{"min", l.min}, {"max", l.max}, {"n", l.n}};
        }
        
        void from_json(const json& j, LinSpace& l){
            j.at("min").get_to(l.min);
            j.at("max").get_to(l.max);
            j.at("n").get_to(l.n);
        }

        void to_json(json& j, const GridSpace& g){
                to_json(j["x"], g.x);
                to_json(j["y"], g.y);
                to_json(j["z"], g.z);
        }

        void from_json(const json& j, GridSpace& g){
            from_json(j["x"], g.x);
            from_json(j["y"], g.y);
            from_json(j["z"], g.z);
        }
        //-------------------------------------------------------------------//
        */


    }
}
