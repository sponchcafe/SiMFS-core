#include <iostream>
#include "diffusion/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"
#include <sstream>

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Diffusion dif;

    //-Configure-------------------------------------------------------------//
    dif.set_json(params);
    dif.set_coordinate_output< file_io::FileOutput >(); // template spec.
    dif.set_collision_output<  file_io::FileOutput >();  // template spec.

    //-Initialize------------------------------------------------------------//
    dif.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(dif.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        dif.run();
    }

}
