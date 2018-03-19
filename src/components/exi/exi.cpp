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
