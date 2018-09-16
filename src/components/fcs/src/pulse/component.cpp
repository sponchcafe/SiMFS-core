#include <algorithm> // std::transform
#include "pulse/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Pulse::Pulse() { } 

        //-------------------------------------------------------------------//
        void Pulse::set_input_id(std::string id){
            input_id = id;
        }
        void Pulse::set_output_id(std::string id){
            output_id = id;
        }
        void Pulse::set_pulse( double rep, std::vector<double> t, std::vector<double> a){

            if (t.size() != a.size()){
                // ERROR LOG
                std::cerr << "Pulse times and powers have unequal number of points.\n";
                return;
            }
            if (t.back() >= rep){
                // ERROR LOG
                std::cerr << "Pulse sequence does not fit into repetition window.\n";
                std::cerr << t.back() << " >= " << rep << "\n";
                return;
            }

            pulse_time = t;
            pulse_power = a;
            repetition_time = rep;
            normalize_pulse();

        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void Pulse::set_json(json j) {

            json params = get_json();
            params.merge_patch(j);

            set_input_id(params.at("input"));
            set_output_id(params.at("output"));
            double rep = params.at("repetition_time");
            std::vector<double> t = params.at("time");
            std::vector<double> a = params.at("power");
            set_pulse(rep, t, a);
                
        }

        //-------------------------------------------------------------------//
        json Pulse::get_json() {

            json j;

            j["input"] = input_id;
            j["output"] = output_id; 
            j["repetition_time"] = repetition_time;
            j["time"] = pulse_time;
            j["power"] = pulse_power;

            return j;
        }

        //-------------------------------------------------------------------//
        void Pulse::init() {
            input_ptr = std::make_unique<io::BufferInput<TimedValue>>(input_id);
            output_ptr = std::make_unique<io::BufferOutput<TimedValue>>(output_id);
        }

        //-------------------------------------------------------------------//
        void Pulse::normalize_pulse() {

            pulse_time.push_back(repetition_time);

            // x: [t0, t1, t2, ..., trep]
            // y: [a0, a1, ..., a_N, a_N]
            
            // integration
            double sum = 0;
            auto it_x = pulse_time.begin();
            for (auto it_y: pulse_power){
                double delta = (*(it_x+1)-(*it_x));
                sum += (it_y) * delta; 
                ++it_x;
                ++it_y;
            } 

            // scaling
            std::transform (pulse_power.begin(), pulse_power.end(), pulse_power.begin(),
                    [=](double a){ return a/sum*repetition_time; }
                    );

            pulse_time.pop_back();
            
        }

        //-------------------------------------------------------------------//
        void Pulse::run() {

            auto it_t = pulse_time.begin();
            auto it_p = pulse_power.begin();

            TimedValue flux_in{0.0, 0.0};
            TimedValue flux_out{0.0, 0.0};

            input_ptr->get(flux_in);
            flux_out = flux_in;
            flux_out.value = flux_in.value * *it_p;

            size_t count = 0;
            bool done = false;

            while(true){

                flux_out.time = count * repetition_time + *it_t;

                while (flux_out.time >= input_ptr->peek().time && !done){

                    // get_the previous power
                    auto it_p_prev = it_p-1;
                    if (it_p_prev < pulse_power.begin()){
                        it_p_prev = pulse_power.end()-1;
                    }

                    done = !input_ptr->get(flux_in);
                    TimedValue flux_intermediate = flux_in;
                    flux_intermediate.value *= *it_p_prev;
                    output_ptr->put(flux_intermediate);

                }

                flux_out.value = flux_in.value * *it_p;
                output_ptr->put(flux_out);

                ++it_t;
                ++it_p;

                if(it_t == pulse_time.end()) {
                    if (done) return;
                    it_t = pulse_time.begin();
                    it_p = pulse_power.begin();
                    count++;
                }

            }
        }

        /*
            while(true){

                flux.time = count * repetition_time + *it_t;
                
                while (flux.time >= coordinate_input_ptr->peek().t) {
                    if(!coordinate_input_ptr->get(c)) return;
                    auto it_p_prev = it_p-1;
                    if (it_p_prev < pulse_power.begin()) it_p_prev = pulse_power.end()-1;

                    // put a diffusion flux update
                    // c is current
                    // it_p is last
                    
                    TimedValue dif_flux{
                        focus_ptr->evaluate(c.x, c.y, c.z) * *it_p_prev * const_scaling,
                            c.t
                    };
                    if (dif_flux.value != flux.value){
                        flux_output_ptr->put(dif_flux);
                    }
                } 

                flux.value = focus_ptr->evaluate(c.x, c.y, c.z) * *it_p * const_scaling;
                flux_output_ptr->put(flux);

                ++it_t;
                ++it_p;

                if(it_t == pulse_time.end()) {
                    it_t = pulse_time.begin();
                    it_p = pulse_power.begin();
                    count++;
                }

            }

        }
        */


    }
}


