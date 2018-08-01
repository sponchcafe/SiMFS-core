#include <iostream>
#include "conformation/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Conformation cnf;

    //-Configure-------------------------------------------------------------//
    cnf.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = cnf.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto cnf_thr = comp::run_component(cnf);
        output_thr.join();
        cnf_thr.join();
    }

}
