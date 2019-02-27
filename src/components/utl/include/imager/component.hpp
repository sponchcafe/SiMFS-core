#pragma once

#include "component/component.hpp"
#include "grid/grid.hpp"
#include <vector>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Imager : public Component{

            public:

                //-----------------------------------------------------------//
                Imager ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_grid_space(sim::grid::GridSpace gspace);

                void set_time_input_ids(std::vector<std::string> ids);
                void set_coordinate_input_ids(std::vector<std::string> ids);
                void set_output_file(std::string fn);
                //-----------------------------------------------------------//


            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                std::string fname = "__image__";
                std::vector<std::string> time_input_ids = {"__timetags__"};
                std::vector<std::string> coordinate_input_ids = {"__coordinates__"};

                sim::grid::GridSpace grid_space{
                    sim::grid::LinSpace{-1e-6, 1e-6, 101},
                    sim::grid::LinSpace{-1e-6, 1e-6, 101},
                    sim::grid::LinSpace{-1e-6, 1e-6, 101}
                };
                sim::grid::Grid<unsigned int> grid{};

                typedef struct {
                    std::unique_ptr<io::BufferInput<sim::Coordinate>> coord_ptr;
                    std::unique_ptr<io::BufferInput<sim::realtime_t>> time_ptr;
                } InputPair;

                std::vector<InputPair> inputs;

                //-----------------------------------------------------------//
                // private functions
                //-----------------------------------------------------------//
                //
                // These functions operate on inputs.begin()
                void find_next_input_pair();
                void image_n_timetags(size_t n);
                void remove_input_pair();

                void add_to_grid(sim::Coordinate, unsigned int count);
                void write_grid_to_file();


                
        };


    }
}
