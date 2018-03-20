#include "shellutils/shellutils.hpp"

const std::string helpmessage = R"(
Produce a single quartett of x,y,z,t coordinates.
)";

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, 
        {"SIM_PARAMS", "SIM_PUT_PARAMS"}
    };
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();

    //----------------------------------------------------------------------//
    double x = spec.get_option<double>
        (
         "x-pos", {"x", "X"}, 0.0,
         "X coordinate in m"
        );

    //----------------------------------------------------------------------//
    double y = spec.get_option<double>
        (
         "y-pos", {"y", "Y"}, 0.0,
         "Y coordinate in m"
        );

    //----------------------------------------------------------------------//
    double z = spec.get_option<double>
        (
         "z-pos", {"z", "Z"}, 0.0,
         "Z coordinate in m"
        );

    //----------------------------------------------------------------------//
    double t = spec.get_option<double>
        (
         "time", {"t", "X"}, 1.0,
         "Dwell time in seconds"
        );

    //----------------------------------------------------------------------//
    std::string out_filename = spec.get_option<std::string>
        (
         "output", {"o", "O"}, jcli::EMPTY_STRING,
         "Output file (coordinates), defaults to stdout"
        );

    spec.enable_help(); // --help
    spec.enable_config(); // --config
    spec.enable_debug(); // --debug
    spec.enable_log(); // --log

    sim::io::Output<sim::Coordinate> out(out_filename);
    sim::Coordinate c{x, y, z, t};

    out.put(c);

}
