#include <iostream>
#include "splitter/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Splitter spl;

    //-Configure-------------------------------------------------------------//
    spl.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = spl.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        auto efficiency_input_thr = io::file2buffer_thread<TimedValue>(log["efficiency_input"]);
        auto photon_input_thr = io::file2buffer_thread<realtime_t>(log["photon_input"]);
        auto accepted_output_thr = io::buffer2file_thread<realtime_t>(log["accepted_output"]);
        auto rejected_output_thr = io::buffer2file_thread<realtime_t>(log["rejected_output"]);
        auto spl_thr = comp::run_component<comp::Splitter>(spl, true);

        efficiency_input_thr.join();
        photon_input_thr.join();
        spl_thr.join();
        rejected_output_thr.join();
        accepted_output_thr.join();
    }

}
