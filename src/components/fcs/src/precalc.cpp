#include "precalc/component.hpp"
#include "efield/storage.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        EFieldPrecalculator::EFieldPrecalculator() {
        }

        //-------------------------------------------------------------------//
        void EFieldPrecalculator::set_filename(std::string fn){
            fname = fn;
        }
        void EFieldPrecalculator::set_gridspec_x(double x_min, double x_max, size_t n){
            spec.x.min = x_min;
            spec.x.max = x_max;
            spec.x.n = n;
        }
        void EFieldPrecalculator::set_gridspec_y(double y_min, double y_max, size_t n){
            spec.y.min = y_min;
            spec.y.max = y_max;
            spec.y.n = n;
        }
        void EFieldPrecalculator::set_gridspec_z(double z_min, double z_max, size_t n){
            spec.z.min = z_min;
            spec.z.max = z_max;
            spec.z.n = n;
        }

        //-------------------------------------------------------------------//
        void EFieldPrecalculator::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_filename(params.at("filename"));
            json grid = params.at("grid");
            json x = grid.at("x");
            json y = grid.at("y");
            json z = grid.at("z");
            set_gridspec_x(x.at("min"), x.at("max"), x.at("n"));
            set_gridspec_y(y.at("min"), y.at("max"), y.at("n"));
            set_gridspec_z(z.at("min"), z.at("max"), z.at("n"));
            json field_spec = params.at("field");
            field_function.set_json(field_spec);
            spec.wavelength = field_function.get_json().at("wavelength");

        }

        //-------------------------------------------------------------------//
        json EFieldPrecalculator::get_json(){

            json j;

            j["filename"] = fname;
            j["grid"] = json {
                {"x", {{"min", spec.x.min}, {"max", spec.x.max}, {"n", spec.x.n}}},
                {"y", {{"min", spec.y.min}, {"max", spec.y.max}, {"n", spec.y.n}}},
                {"z", {{"min", spec.z.min}, {"max", spec.z.max}, {"n", spec.z.n}}}
            };
            j["field"] = field_function.get_json();
            return j;

        }

        //-------------------------------------------------------------------//
        void EFieldPrecalculator::init() {
            field_function.init();
        }
            

        //-------------------------------------------------------------------//
        void EFieldPrecalculator::run(){
            std::vector<field::EFieldCoordinate> c{};
            std::vector<field::EFieldComponents> f{};
            if (field::check_file_exists(fname)) {
                std::cerr << "File " << fname << " exists, aborting.\n";
                return;
            }
            field::evaluate_prefactors(field_function, spec);
            field::make_grid_coordinates(spec, c);
            field::evaluate_efield_grid(field_function, c, f);
            field::save_field_file(fname, spec, f);
        }



    }
}
