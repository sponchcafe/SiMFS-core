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


/*
#include "shellutils/shellutils.hpp"
#include "focus/alpha_gauss.hpp"
#include "focus/gaussian.hpp"

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, 
        {"SIM_PARAMS", "SIM_EXI_PARAMS"}
    };
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();  // --scope

    //----------------------------------------------------------------------//
    std::string infile = spec.get_option<std::string>(
            "input", {"i", "I"}, "",
            "Input file stream of (x,y,z,t) coordinates."
            );

    //----------------------------------------------------------------------//
    std::string outfile = spec.get_option<std::string>(
            "output", {"o", "O"}, "",
            "Output file stream of photon flux."
            );

    //----------------------------------------------------------------------//
    double power = spec.get_option<double>(
            "power", {"p", "P"}, 1e-6,
            "Excitation power in Watt.",
            [] (double d) -> bool { return d>0;}
            );

    //----------------------------------------------------------------------//
    double wavelength = spec.get_option<double>(
            "wavelength", {"l", "L"}, 488e-9,
            "Excitation wavelength in meter.",
            [] (double d) -> bool { return d>0;}
            );

    //----------------------------------------------------------------------//
    double waist_x = spec.get_option<double>(
            "waist-x", {"x", "X"}, 100e-9,
            "1/e^2 decay of the focus function in x in meter.",
            [] (double d) -> bool { return d>0;}
            );

    //----------------------------------------------------------------------//
    double waist_y = spec.get_option<double>(
            "waist-y", {"y", "Y"}, 100e-9,
            "1/e^2 decay of the focus function in y in meter.",
            [] (double d) -> bool { return d>0;}
            );

    //----------------------------------------------------------------------//
    double waist_z = spec.get_option<double>(
            "waist-z", {"z", "Z"}, 100e-9,
            "1/e^2 decay of the focus function in z in meter.",
            [] (double d) -> bool { return d>0;}
            );
    
    //----------------------------------------------------------------------//
    double offset_x = spec.get_option<double>(
            "offset-x", {"u", "U"}, 0.0,
            "Offset in x direction in meter."
            );

    //----------------------------------------------------------------------//
    double offset_y = spec.get_option<double>(
            "offset-y", {"v", "V"}, 0.0,
            "Offset in y direction in meter."
            );

    //----------------------------------------------------------------------//
    double offset_z = spec.get_option<double>(
            "offset-z", {"w", "W"}, 0.0,
            "Offset in z direction in meter."
            );

    //----------------------------------------------------------------------//
    json shape = spec.get_option<json>(
            "shape", {"a", "A"}, json{
                {"offset", {0.0}},
                {"scaling", {1.0}}
                },
            "Structured shape specification (json object): {'offset':[], 'scaling':[]}",
            [] (json j) -> bool {
                if (!j.is_object()) return false;
                if (!j.count("offset")) return false;
                if (!j.count("scaling")) return false;
                if (!j["offset"].is_array()) return false;
                if (!j["scaling"].is_array()) return false;
                if (j["offset"].size() != j["scaling"].size()) return false;
                for (auto it: j["offset"]) if (!it.is_number() || double(it)<0) return false;
                for (auto it: j["scaling"]) if (!it.is_number() || double(it)<0) return false;
                return true;
            }
            );

    std::vector<double> shape_offset = shape["offset"];
    std::vector<double> shape_scaling = shape["scaling"];
    std::vector<double> shape_power = shape_scaling;
    std::transform(
            shape_scaling.begin(), 
            shape_scaling.end(),
            shape_power.begin(),
            [power](double s) -> double { return power * s;}
            );
            
    //----------------------------------------------------------------------//
    std::string type = spec.get_option<std::string>(
            "type", {"t", "T"}, "gauss",
            "Type of the focus form. [gauss | alpha]",
            [] (std::string s) -> bool { return (s=="gauss" || s=="alpha");}
            );

    //----------------------------------------------------------------------//
    std::unique_ptr<sim::focus::BaseFocus> focus_ptr;
    if (type == "gauss"){
        focus_ptr.reset(new sim::focus::Gaussian(waist_x, waist_y, waist_z));
    }
    else if (type == "alpha"){
        focus_ptr.reset(new sim::focus::AlphaGauss(waist_x, waist_z));
    }
    else{
        std::cerr << "Focus type not recognized: " << type << std::endl;
        exit(-1);
    }

    spec.enable_config();   // --config
    spec.enable_log();   // --log
    spec.enable_debug();    // --debug
    spec.enable_help();     // --help

    sim::io::Input <sim::Coordinate> input(infile);
    sim::io::Output<sim::TimedValue> output(outfile);

    sim::Coordinate c{0.0,0.0,0.0,0.0};
    sim::TimedValue flux{0.0, 0.0};

    double const_wavelength_scaling = wavelength / (sim::CONST_H * sim::CONST_C);

    while(input.get(c)){
        double F = focus_ptr->evaluate(c.x+offset_x, c.y+offset_y, c.z+offset_z);
        for (unsigned i=0; i<shape_offset.size(); i++){
            flux.time = c.t + shape_offset[i]; 
            flux.value = F * shape_power[i] * const_wavelength_scaling;
            output.put(flux);
        }
    }
    
}
*/
