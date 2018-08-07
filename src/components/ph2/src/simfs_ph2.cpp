#include <iostream>
#include "photophysics/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;
using namespace sim::graph;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Photophysics ph2;

    //-Configure-------------------------------------------------------------//
    ph2.set_json(params);

    //-Init------------------------------------------------------------------//
    ph2.init();

    //-Log-------------------------------------------------------------------//
    json log = ph2.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        std::vector<std::thread> io_threads{};
        for (auto &id: ph2.get_timetag_output_ids()){
            io_threads.push_back(io::buffer2file_thread<realtime_t>(id));
        }
        for (auto &id: ph2.get_flux_input_ids()){
            io_threads.push_back(io::file2buffer_thread<TimedValue>(id));
        }
        auto ph2_thread = comp::run_component(ph2);

        ph2_thread.join();
        for (auto &th: io_threads) th.join();
    }

}
