#include "excitation/component.hpp"
#include "focus/alpha.hpp"
#include <algorithm> // std::transform

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        Excitation::Excitation() {
            focus_ptr = std::make_unique<focus::Alpha>();
        }

        //-------------------------------------------------------------------//
        void Excitation::set_coordinate_input(std::string id){
            set_coordinate_input_id(id);
        }
        void Excitation::set_flux_output(std::string id){
            set_flux_output_id(id);
        }
        void Excitation::set_wavelength(double w){
            wavelength = fabs(w);
        }
        void Excitation::set_power(double p){
            power = fabs(p);
        }
        void Excitation::set_coordinate_input_id(std::string id){
            coordinate_input_id = id;
        }
        void Excitation::set_flux_output_id(std::string id){
            flux_output_id = id;
        }
        void Excitation::set_pulse(
                double rep, 
                std::vector<double> &t,
                std::vector<double> &a){

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

        }
        void Excitation::set_focus(json params){
            if (params.at("type") == "alpha"){
                focus_ptr = std::make_unique<focus::Alpha>();
                focus_ptr->set_json(params);
            }
            else {
                // ERROR LOG
                std::cerr << "Focus type '" << params.at("type") << "' not recognized.\n";
            }
        }
        //-------------------------------------------------------------------//


        //-------------------------------------------------------------------//
        void Excitation::set_json(json j) {
            
            // check for cw case:
            if (j.find("pulse") == j.end()) {
                std::cerr << "CW mode\n";
                cw = true;
            }
            else{
                std::cerr << "pulsed mode\n";
                cw = false;
            }

            json params = get_json();
            params.merge_patch(j);

            set_wavelength(params.at("wavelength"));
            set_power(params.at("power"));
            set_focus(params.at("focus"));
            set_coordinate_input_id(params.at("coordinate_input"));
            set_flux_output_id(params.at("flux_output"));
            if (!cw) {
                json pulse = params.at("pulse");
                double rep = pulse.at("repetition_time");
                std::vector<double> t = pulse.at("time");
                std::vector<double> a = pulse.at("power");
                set_pulse(rep, t, a);
            }
                
        }

        //-------------------------------------------------------------------//
        json Excitation::get_json() {
            json j;

            j["wavelength"] = wavelength;
            j["power"] = power;
            j["coordinate_input"] = coordinate_input_id;
            j["flux_output"] = flux_output_id;
            if (!cw){
                j["pulse"] = json{
                    {"repetition_time", repetition_time},
                    {"time", pulse_time}, 
                    {"power", pulse_power}
                };
            }
            j["focus"] = focus_ptr->get_json();

            return j;
        }

        //-------------------------------------------------------------------//
        void Excitation::init() {
            normalize_pulse();
            coordinate_input_ptr = 
                std::make_unique<io::BufferInput<Coordinate>>(coordinate_input_id);
            flux_output_ptr = 
                std::make_unique<io::BufferOutput<TimedValue>>(flux_output_id);
        }

        //-------------------------------------------------------------------//
        void Excitation::run() {
            if (cw){
                run_cw();
            } else{
                run_pulsed();
            }
        }

        //-------------------------------------------------------------------//
        void Excitation::run_cw() {
            double const_scaling = power * wavelength / (CONST_C * CONST_H);
            while(coordinate_input_ptr->get(c)){
                flux.time = c.t;
                flux.value = focus_ptr->evaluate(c.x, c.y, c.z)*const_scaling;
                flux_output_ptr->put(flux);
            }
        }

        //-------------------------------------------------------------------//
        void Excitation::run_pulsed() {

            coordinate_input_ptr->get(c);
            flux.time = c.t;

            auto it_t = pulse_time.begin();
            auto it_p = pulse_power.begin();

            unsigned long count = 0;
            double const_scaling = wavelength / (CONST_C * CONST_H);

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

        //-------------------------------------------------------------------//
        void Excitation::normalize_pulse() {
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
            double scaling = power*repetition_time/sum;
            std::transform (pulse_power.begin(), pulse_power.end(), pulse_power.begin(),
                    [=](double a){ return a*scaling; }
                    );

            pulse_time.pop_back();
        }

    }

}


