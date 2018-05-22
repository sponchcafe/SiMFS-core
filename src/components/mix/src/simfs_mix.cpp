#include <iostream>
#include "mixer/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Mixer mix;

    //-Configure-------------------------------------------------------------//
    mix.set_json(params);
    mix.set_photon_output< file_io::FileOutput >(); // template spec.
    mix.set_photon_inputs< file_io::FileInput >(); // template spec.

    //-Initialize------------------------------------------------------------//
    mix.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(mix.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        mix.run();
    }

}
