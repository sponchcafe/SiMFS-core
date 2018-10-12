#pragma once

#include "component/component.hpp"
#include <thread>
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
                void set_gridspec_x(double min, double max, size_t n);
                void set_gridspec_y(double min, double max, size_t n);
                void set_gridspec_z(double min, double max, size_t n);
                void set_time_input_ids(std::vector<std::string> ids);
                void set_coordinate_input_ids(std::vector<std::string> ids);
                void set_output_file(std::string fn);
                void set_value_mode(bool v);
                //-----------------------------------------------------------//

                bool in_value_mode();
               

            private:


                //-----------------------------------------------------------//
                struct GridValue{
                    int x;
                    int y;
                    int z;
                    double v;
                };

                //-----------------------------------------------------------//
                struct GridDimension{
                    double min;
                    double max;
                    unsigned long long n;
                };

                //-----------------------------------------------------------//
                struct GridSpec{
                    GridDimension x;
                    GridDimension y;
                    GridDimension z;
                };

                //-----------------------------------------------------------//
                double x_step;
                double y_step;
                double z_step;

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//

                std::string fname = "__image__";
                std::vector<std::string> time_input_ids = {"__timetags__"};
                std::vector<std::string> coordinate_input_ids = {"__coordinates__"};
                
                bool value_mode = false;

                GridSpec grid_spec {
                    {-1e-6, 1e-6, 101},
                    {-1e-6, 1e-6, 101},
                    {-1e-6, 1e-6, 101}
                };

                std::vector<double> grid;

                //-----------------------------------------------------------//
                // private functions
                //-----------------------------------------------------------//
                GridValue coordinate_to_grid(Coordinate c);
                bool image_chunk(
                        io::BufferInput<Coordinate> &coords, 
                        io::BufferInput<realtime_t> &tags);
                void add_to_grid(GridValue gridv);
                void add_to_grid(Coordinate c, double value);
                void write_file();

                const static unsigned int CHUNK_SIZE = 1;
                
        };


    }
}
