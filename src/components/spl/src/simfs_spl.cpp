#include <iostream>
#include "splitter/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Splitter spl;

    //-Configure-------------------------------------------------------------//
    spl.set_json(params);

    //-Connect-io------------------------------------------------------------//
    spl.set_photon_input< file_io::FileInput > (); // template spec.
    spl.set_efficiency_input< file_io::FileInput > ();  // template spec.
    spl.set_accepted_photon_output< file_io::FileOutput > (); // template spec.
    spl.set_rejected_photon_output< file_io::FileOutput > (); // template spec.

    //-Initialize------------------------------------------------------------//
    spl.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(spl.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        spl.run();
    }

}
