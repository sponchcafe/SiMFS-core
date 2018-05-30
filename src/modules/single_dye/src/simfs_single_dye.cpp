#include <iostream>
#include "single_dye/module.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::SingleDye single_dye;

    //-Configure-------------------------------------------------------------//
    single_dye.set_json(params);
    single_dye.set_photon_output< file_io::FileOutput >();

    //-Initialize------------------------------------------------------------//
    single_dye.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(single_dye.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        single_dye.run();
    }

}
