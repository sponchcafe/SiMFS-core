#include "imager/component.hpp"
#include "io/buffer.hpp"
#include <algorithm>
#include <iterator>
#include <cmath>
#include <fstream>

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
        void Imager::set_grid_space(sim::grid::GridSpace gspace){
            grid_space = gspace;
        }
        //-------------------------------------------------------------------//
        
        
        //-------------------------------------------------------------------//
        void Imager::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_time_input_ids(params.at("time_inputs"));
            set_coordinate_input_ids(params.at("coordinate_inputs"));
            set_output_file(params.at("output_file"));

            sim::grid::GridSpace gs;
            from_json(params.at("grid"), gs);
            set_grid_space(gs);

        }

        //-------------------------------------------------------------------//
        json Imager::get_json(){

            json j;

            j["time_inputs"] = time_input_ids;
            j["coordinate_inputs"] = coordinate_input_ids;
            j["output_file"] = fname;

            to_json(j["grid"], grid_space);

            return j;

        }


        //-------------------------------------------------------------------//
        void Imager::init() {

            for (size_t i=0; i<time_input_ids.size(); i++) {

                auto time_id = time_input_ids[i];
                auto coord_id = coordinate_input_ids[i];

                inputs.emplace_back(
                    InputPair{
                        std::make_unique<io::BufferInput<Coordinate>>(coord_id),
                        std::make_unique<io::BufferInput<realtime_t>>(time_id)
                    }
                );
            }

            grid = sim::grid::Grid<unsigned int>(grid_space);

        }

        //-------------------------------------------------------------------//
        void Imager::remove_input_pair(){
            Coordinate c;
            realtime_t t;
            while(inputs.begin()->coord_ptr->get(c)); // safety drain
            while(inputs.begin()->time_ptr->get(t)){ // safety drain
                std::cerr << "DISCARDING t=" << t <<'\n';
            }
            std::swap(*inputs.begin(), *(inputs.end()-1));
            inputs.pop_back();
        }

        //-------------------------------------------------------------------//
        void Imager::find_next_input_pair(){

            /* Linear search for the input with the max amount of timetags.
             * The max input is swapped to the beginning of the input vector.
             **/

            size_t max_size = 0;
            auto max = inputs.begin();
            auto cur = inputs.begin();

            while(cur != inputs.end()){
                auto size = cur->time_ptr->get_size();
                if(size >= max_size){
                    max_size = size;
                    max = cur;
                }
                ++cur;
            }

            std::swap(*inputs.begin(), *max);

        }

        //-------------------------------------------------------------------//
        void Imager::add_to_grid(sim::Coordinate c, unsigned int count){
            sim::grid::Coordinate gc{c.x, c.y, c.z};
            if (!grid.check_inside(gc)) return;
            grid.set(gc, count, [=](unsigned int c0, unsigned int c1){
                    return c0+c1;
                    });
        }

        //-------------------------------------------------------------------//
        void Imager::image_available_timetags(){

            // Always work with the current first input pair
            auto &times = inputs.begin()->time_ptr;
            auto &coords= inputs.begin()->coord_ptr;

            size_t n_available = times->get_size();
            std::cerr << "N available: " << n_available << '\n';

            if (n_available == 0){
                n_available = 1;
            }
            
            while(n_available > 0){

                Coordinate c{};
                realtime_t t{};
                unsigned int count = 0;

                // FFwd to coordinate
                while( fabs(times->peek()) >= coords->peek().t) {
                    if(!coords->get(c)) {
                        // no more coorindates -> count the remaining tags, 
                        // add to the grid and return
                        while(times->get(t) && !std::signbit(t)) count++;
                        add_to_grid(c, count);
                        remove_input_pair();
                        return;
                    }
                }

                // accumulate tags for current coordinate
                while( fabs(times->peek()) < coords->peek().t) {

                    if (times->is_done()){
                        add_to_grid(c, count);
                        remove_input_pair();
                        return;
                    }


                    times->get(t);
                    if (!std::signbit(t)) ++count;
                    --n_available;

                    if(n_available == 1) break;

                }

                add_to_grid(c, count);

            }

        }


        //-------------------------------------------------------------------//
        void Imager::write_grid_to_file(){

            std::fstream fs(fname, std::fstream::out | std::iostream::binary);

            if (!fs.good()){
                std::cerr << "Error opening file\n";
                std::exit(0);
            }
            sim::grid::GridSerializer<unsigned int> gs(fs, grid);

            gs.serialize();
        }
        

        //-------------------------------------------------------------------//
        void Imager::run(){

            while(inputs.size() > 0){

                // get in_it point to the next input to work on
                find_next_input_pair();

                // image all but the last photon to avoid blocking
                image_available_timetags();

            }

            // write results to file
            write_grid_to_file();

        }

    }
}


