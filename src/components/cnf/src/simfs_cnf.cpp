#include <iostream>
#include "conformation/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Conformation cnf;

    //-Configure-------------------------------------------------------------//
    cnf.set_json(params);

    //-Connect-io------------------------------------------------------------//
    cnf.set_value_output < file_io::FileOutput > ();

    //-Initialize------------------------------------------------------------//
    cnf.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(cnf.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        cnf.run();
    }

}
