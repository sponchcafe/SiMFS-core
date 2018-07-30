#include <iostream>
#include "detection/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Detection det;

    //-Configure-------------------------------------------------------------//
    det.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = det.get_json();
    cli::log_parameters(log);


    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["coordinate_input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["efficiency_output"]);
        auto det_thr = comp::run_component<comp::Detection>(det);
        input_thr.join();
        det_thr.join();
        output_thr.join();
    }

}
