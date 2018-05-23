#include <iostream>
#include "buffer/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Buffer buf;

    //-Configure-------------------------------------------------------------//
    buf.set_json(params);
    buf.set_input< file_io::FileInput >(); // template spec.
    buf.set_outputs< file_io::FileOutput >(); // template spec.

    //-Initialize------------------------------------------------------------//
    buf.init();

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(buf.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        buf.run();
    }

}
