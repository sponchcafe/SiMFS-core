#include <iostream>
#include "excitation/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Excitation exi;

    //-Configure-------------------------------------------------------------//
    exi.set_json(params);
    exi.set_coordinate_input< file_io::FileInput >(); // template spec.
    exi.set_flux_output<  file_io::FileOutput >();  // template spec.

    //-Initialize------------------------------------------------------------//
    exi.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(exi.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        exi.run();
    }

}
