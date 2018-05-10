#include "diffusion/component.hpp"
#include <exception>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Diffusion::Diffusion(){ 
            set_seed(random::get_new_seed());
        }

        //-------------------------------------------------------------------//
        void Diffusion::set_diffusion_coefficient(double d) { 
            diffusion_coefficient = fabs(d);
        }
        void Diffusion::set_radius(double r) { 
            radius = fabs(r);
        }
        void Diffusion::set_half_height(double h) { 
            half_height = fabs(h);
        }
        void Diffusion::set_experiment_time(double t) { 
            experiment_time = fabs(t); 
        }
        void Diffusion::set_increment(double i) { 
            increment = fabs(i);
        }
        void Diffusion::set_seed(unsigned s) { 
            seed = s; 
        }
        void Diffusion::set_coordinate_output(std::string id){
            coordinate_output_id = id;
        }
        void Diffusion::set_collision_output(std::string id){
            collision_output_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void Diffusion::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            set_diffusion_coefficient(params.at("diffusion_coefficient"));
            set_experiment_time(params.at("experiment_time"));
            set_radius(params.at("radius"));
            set_half_height(params.at("half_height"));
            set_increment(params.at("increment"));
            set_seed(params.at("seed"));
            set_coordinate_output(params.at("coordinate_output"));
            set_collision_output(params.at("collision_output"));

        }

        //-------------------------------------------------------------------//
        json Diffusion::get_json(){

            json j;

            j["diffusion_coefficient"] = diffusion_coefficient;
            j["experiment_time"] = experiment_time;
            j["increment"] = increment;
            j["radius"] = radius;
            j["half_height"] = half_height;
            j["seed"] = seed;
            j["coordinate_output"] = coordinate_output_id;
            j["collision_output"] = collision_output_id;

            return j;

        }


        //-------------------------------------------------------------------//
        void Diffusion::init() {
            steps = (unsigned long long) floor(experiment_time/increment);
            sigma = sqrt(2 * diffusion_coefficient * increment);
            normal = random::Normal{sigma, seed};
            uni = random::Uniform{seed};
            c0 = get_initial_coordinate();
        }

        //-------------------------------------------------------------------//
        void Diffusion::run(){

            unsigned long long i = 0;
            while(i<steps){

                c1.x = c0.x + normal();
                c1.y = c0.y + normal();
                c1.z = c0.z + normal();

                if (check_within_box(c1)){
                    c0 = c1;
                    c0.t = increment * i;
                    coordinate_output_ptr->put(c0);
                    i++;
                }
                else{
                    if (c0.t != last_reset){
                        collision_output_ptr->put(c0);
                        last_reset = c0.t;
                    }
                }
            }
        }

        //-------------------------------------------------------------------//
        bool Diffusion::check_within_box (Coordinate &c){
            bool in_xy = sqrt(pow(c.x, 2)+pow(c.y, 2)) < radius;
            bool in_z = fabs(c.z) < half_height;
            return in_xy && in_z;
        }

        //-------------------------------------------------------------------//
        Coordinate Diffusion::get_initial_coordinate(){
            Coordinate c_init;
            do{
                c_init.x = (uni()*2 - 1)*radius;
                c_init.y = (uni()*2 - 1)*radius;
                c_init.z = (uni()*2 - 1)*half_height;
            } while(!check_within_box(c_init));
            return c_init;
        }

    }
}
