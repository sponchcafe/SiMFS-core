#include "shellutils/shellutils.hpp"
#include "focus/alpha_gauss.hpp"
#include "focus/gaussian.hpp"
#include "random/random.hpp"

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, 
        {"SIM_PARAMS", "SIM_DET_PARAMS"}
    };
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();  // --scope

    //----------------------------------------------------------------------//
    std::string photons_file = spec.get_option<std::string>(
            "photons", {"p", "P"}, "",
            "Input file stream of timetags."
            );

    //----------------------------------------------------------------------//
    std::string coords_file= spec.get_option<std::string>(
            "coordinates", {"c", "C"}, "",
            "Input file stream of (x, y, z, t) coordinates."
            );

    //----------------------------------------------------------------------//
    std::string output_file = spec.get_option<std::string>(
            "output", {"o", "O"}, "",
            "Output file stream of detected photons."
            );

    //----------------------------------------------------------------------//
    std::string type = spec.get_option<std::string>(
            "type", {"t", "T"}, "gauss",
            "Type of the focus form. [gauss | alpha]",
            [] (std::string s) -> bool { return (s=="gauss" || s=="alpha");}
            );

    //----------------------------------------------------------------------//
    double max_efficiency= spec.get_option<double>(
            "max-efficiency", {"m", "M"}, 1.0, 
            "Maximum efficiency set at (0, 0, 0)",
            [] (double d) -> bool { return (d >= 0 && d <= 1);}
            );

    //----------------------------------------------------------------------//
    unsigned seed= spec.get_option<unsigned>(
            "seed", {"s", "S"}, sim::random::get_new_seed(),
            "Random seed"
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

    // scaling factor for focus evaluation
    double scaling = 1/focus_ptr->evaluate(0,0,0) * max_efficiency;

    spec.enable_config();   // --config
    spec.enable_log();      // --log
    spec.enable_debug();    // --debug
    spec.enable_help();     // --help

    sim::io::Input<sim::Coordinate> coordinates(coords_file);
    sim::io::Input<sim::realtime_t> photons(photons_file);
    sim::io::Output<sim::realtime_t> output(output_file);

    sim::Coordinate c{0.0,0.0,0.0,0.0};
    sim::realtime_t timetag{0.0};
    double efficiency{0.0};

    sim::random::Uniform uni{seed};

    while(photons.get(timetag)){

        while(timetag > c.t){
            coordinates.get(c);
        }

        efficiency = focus_ptr->evaluate(
                c.x+offset_x, 
                c.y+offset_y, 
                c.z+offset_z)*scaling;

        if (uni() < efficiency){
            output.put(timetag);
        }

    }

    while(coordinates.get(c));

}
