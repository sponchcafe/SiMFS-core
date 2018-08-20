#include <iostream>
#include "gauss/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::FCS_Gauss fcs;

    //-Configure-------------------------------------------------------------//
    fcs.set_json(params);

    if(cli::search_token(opts, "det")) {
        std::cerr << "Detection mode\n";
        fcs.set_normalize(true);
    }

    //-Log-------------------------------------------------------------------//
    json log = fcs.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto exi_thr = comp::run_component<comp::FCS_Gauss>(fcs, true);
        input_thr.join();
        exi_thr.join();
        output_thr.join();
    }

}
