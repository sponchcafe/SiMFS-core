#include <iostream>
#include "detection/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Detection det;

    //-Configure-------------------------------------------------------------//
    det.set_json(params);
    det.set_coordinate_input< file_io::FileInput >(); // template spec.
    det.set_efficiency_output<  file_io::FileOutput >();  // template spec.

    //-Initialize------------------------------------------------------------//
    det.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(det.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        det.run();
    }

}
