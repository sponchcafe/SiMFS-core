#include "component/component.hpp"
#include "component/types.hpp"
#include "random/random.hpp"
#include <cmath>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Diffusion : public Component{

            public:

                //-----------------------------------------------------------//
                Diffusion ();

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_diffusion_coefficient(double d);
                void set_radius(double r);
                void set_half_height(double h);
                void set_experiment_time(double t);
                void set_increment(double i);
                void set_seed(unsigned s);
                void set_coordinate_output(std::string id);
                void set_collision_output(std::string id);
                //-----------------------------------------------------------//
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_coordinate_output(){
                    set_coordinate_output<OutputT>(coordinate_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT> 
                void set_coordinate_output(std::string id){
                    coordinate_output_id = id;
                    coordinate_output_ptr = 
                        create_output<OutputT, Coordinate>(
                            coordinate_output_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_collision_output(){
                    set_collision_output<OutputT>(collision_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_collision_output(std::string id){
                    collision_output_id = id;
                    collision_output_ptr = 
                        create_output<OutputT, Coordinate>(
                            collision_output_id
                        );
                }
                //-----------------------------------------------------------//

            private:

                //-----------------------------------------------------------//
                Coordinate get_initial_coordinate();

                //-----------------------------------------------------------//
                static double get_sigma (double d, double i);

                //-----------------------------------------------------------//
                bool check_within_box (Coordinate &c);

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double diffusion_coefficient = 1e-10;       // m/s^2
                double radius = 1e-6;                       // m
                double half_height = 1e-6;                  // m
                double experiment_time = 1;                 // s
                double increment = 1e-7;                    // s
                unsigned seed = 0;
                std::string coordinate_output_id = "./coords";
                std::string collision_output_id = "/dev/null";

                //-----------------------------------------------------------//
                double sigma = 0;
                unsigned long long steps = 0;
                random::Uniform uni{0};         // seed 0
                random::Normal normal{1, 0};     // sigma 1, seed 0
                realtime_t last_reset = 0;

                //-----------------------------------------------------------//
                Coordinate c0{0.0,0.0,0.0,0.0};
                Coordinate c1{0.0,0.0,0.0,0.0};

                //-----------------------------------------------------------//
                std::unique_ptr<Output<Coordinate>> coordinate_output_ptr;
                std::unique_ptr<Output<Coordinate>> collision_output_ptr;

        };

    }
}