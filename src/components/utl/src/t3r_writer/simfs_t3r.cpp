#include <iostream>
#include "t3r_writer/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::T3rWriter t3r{};

    //-Configure-------------------------------------------------------------//
    t3r.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = t3r.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        auto in_thread = io::file2buffer_thread<RoutedTime>(log["input"]);
        auto t3r_thread = comp::run_component<comp::T3rWriter>(t3r, true);
        
        in_thread.join();
        t3r_thread.join();

    }

}
