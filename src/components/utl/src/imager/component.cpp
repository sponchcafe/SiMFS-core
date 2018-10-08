
#include "imager/component.hpp"
#include "io/buffer.hpp"
#include <algorithm>
#include <iterator>
#include <thread>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Imager::Imager(){ }

        //-------------------------------------------------------------------//
        void Imager::set_time_input_ids(std::vector<std::string> ids){
            time_input_ids = ids;
        }
        void Imager::set_coordinate_input_ids(std::vector<std::string> ids){
            coordinate_input_ids = ids;
        }
        void Imager::set_output_file(std::string fn){
            fname = fn;
        }
        void Imager::set_value_mode(bool v){
            value_mode = v;
        }
        void Imager::set_gridspec_x(double min, double max, size_t n){
            grid_spec.x.min = min;
            grid_spec.x.max = max;
            grid_spec.x.n = n;
        }
        void Imager::set_gridspec_y(double min, double max, size_t n){
            grid_spec.y.min = min;
            grid_spec.y.max = max;
            grid_spec.y.n = n;
        }
        void Imager::set_gridspec_z(double min, double max, size_t n){
            grid_spec.z.min = min;
            grid_spec.z.max = max;
            grid_spec.z.n = n;
        }
        //-------------------------------------------------------------------//
        
        
        //-------------------------------------------------------------------//
        void Imager::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_time_input_ids(params.at("time_inputs"));
            set_coordinate_input_ids(params.at("coordinate_inputs"));
            set_output_file(params.at("output_file"));

            json gridx = params.at("grid").at("x");
            json gridy = params.at("grid").at("y");
            json gridz = params.at("grid").at("z");

            set_gridspec_x(gridx.at("min"), gridx.at("max"), gridx.at("n"));
            set_gridspec_y(gridy.at("min"), gridy.at("max"), gridy.at("n"));
            set_gridspec_z(gridz.at("min"), gridz.at("max"), gridz.at("n"));

        }

        //-------------------------------------------------------------------//
        json Imager::get_json(){

            json j;

            j["time_inputs"] = time_input_ids;
            j["coordinate_inputs"] = coordinate_input_ids;
            j["output_file"] = fname;
            j["grid"]["x"] = {{"min", grid_spec.x.min}, {"max", grid_spec.x.max}, {"n", grid_spec.x.n}};
            j["grid"]["y"] = {{"min", grid_spec.y.min}, {"max", grid_spec.y.max}, {"n", grid_spec.y.n}};
            j["grid"]["z"] = {{"min", grid_spec.z.min}, {"max", grid_spec.z.max}, {"n", grid_spec.z.n}};

            return j;

        }


        //-------------------------------------------------------------------//
        void Imager::init() {

            grid.resize(grid_spec.x.n*grid_spec.y.n*grid_spec.z.n);
            x_step = (grid_spec.x.n > 0) ? (grid_spec.x.max-grid_spec.x.min)/grid_spec.x.n : 0;
            y_step = (grid_spec.y.n > 0) ? (grid_spec.y.max-grid_spec.y.min)/grid_spec.y.n : 0;
            z_step = (grid_spec.z.n > 0) ? (grid_spec.z.max-grid_spec.z.min)/grid_spec.z.n : 0;

        }

        //-------------------------------------------------------------------//
        void Imager::run(){

            // start reading
            std::vector<std::thread> threads;

            auto coords_id_it = coordinate_input_ids.begin();
            auto time_id_it = time_input_ids.begin();

            while(coords_id_it != coordinate_input_ids.end()){

                if (!value_mode){
                    threads.emplace_back(
                        [=] () {
                            this->image_timetags(*coords_id_it, *time_id_it);
                        }
                    );
                }else{
                }

                ++coords_id_it;
                ++time_id_it;

            }

            // start collection
            std::thread t([=] () {this->collect_image();});

            // wait all
            for (auto &t: threads) t.join();
            t.join();

            // write result
            write_file();

        }


        //-------------------------------------------------------------------//
        Imager::GridValue Imager::coordinate_to_grid(Coordinate c){

            int ix = (int) ((c.x-grid_spec.x.min) / x_step);
            int iy = (int) ((c.y-grid_spec.y.min) / y_step);
            int iz = (int) ((c.z-grid_spec.z.min) / z_step);

            return GridValue{ix, iy, iz, 1.0};
        }


        //-------------------------------------------------------------------//
        void Imager::image_timetags(std::string coords_id, std::string tags_id){

            auto coords = std::make_unique<io::BufferInput<Coordinate>>(coords_id);
            auto tags = std::make_unique<io::BufferInput<realtime_t>>(tags_id);
            auto out = std::make_unique<io::BufferOutput<Imager::GridValue>>(coords_id);

            Coordinate c{0,0,0,0.0};
            realtime_t tag{0};

            Imager::GridValue gridv{0,0,0,0.0};
            size_t count = 0;

            while(coords->get(c)){
                count = 0;
                while(tag < c.t){
                    if (!tags->get(tag)) break;
                    count++;
                }
                if (count <= 0) continue;
                gridv = coordinate_to_grid(c);
                gridv.v = (double) count;
                out->put(gridv);
            }

        }

        //-------------------------------------------------------------------//
        void Imager::image_timed_values(std::string coords_id, std::string tvals_id){
            std::cerr << "Value mode not implemented!\n";
        }
        
        //-------------------------------------------------------------------//
        void Imager::add_to_grid(Imager::GridValue gridv){

            int index = 
                gridv.x*grid_spec.y.n*grid_spec.z.n + 
                gridv.y*grid_spec.z.n+ 
                gridv.z;

            if (index < 0 || index > (int) grid.size()) return;

            grid[index] += gridv.v;

        }

        //-------------------------------------------------------------------//
        void Imager::collect_image(){

            Imager::GridValue gridv;
            std::vector<std::unique_ptr<io::BufferInput<Imager::GridValue>>> grid_value_input_ptrs;
            for (auto &id: coordinate_input_ids){
                grid_value_input_ptrs.emplace_back(std::make_unique<io::BufferInput<Imager::GridValue>>(id));
            }
            auto current = grid_value_input_ptrs.begin();
            auto last = grid_value_input_ptrs.end();

            while(grid_value_input_ptrs.size() > 1){

                if(!current->get()->get(gridv)){
                    std::swap(current, last);
                    grid_value_input_ptrs.pop_back();
                    last = grid_value_input_ptrs.end();
                    current = grid_value_input_ptrs.begin();
                    continue;
                }

                add_to_grid(gridv);
                current++;

                if (current == grid_value_input_ptrs.end()){
                    current = grid_value_input_ptrs.begin();
                }

            }

            while(current->get()->get(gridv)){
                add_to_grid(gridv);
            }

        }

        //-------------------------------------------------------------------//
        void Imager::write_file(){

            auto ifs = std::ifstream(fname, std::ifstream::binary);
            if (ifs.good()){
                std::cerr << "Image file " << fname << " already exists.\n";
                ifs.close();
                return;
            } 

            auto fs = std::ofstream(fname, std::ofstream::binary);
            if (!fs.good()){
                fs.close();
                std::cerr << "Failed to open image file: " << fname << "\n";
                return;
            }

            fs.write(reinterpret_cast<char *>(&grid_spec), sizeof(GridSpec));
            fs.write(reinterpret_cast<char *>(grid.data()), grid.size()*sizeof(double));

            fs.close();

            std::cerr << "Image written to " << fname << "\n";
        }

        //-------------------------------------------------------------------//
        bool Imager::in_value_mode(){
            return value_mode;
        }


    }
}
