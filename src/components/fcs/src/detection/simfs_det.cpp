#include "detection/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;

//---------------------------------------------------------------------------//
int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Detection det{};

    //-Log-------------------------------------------------------------------//
    det.set_json(params);
    json log = det.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto det_thr = comp::run_component<comp::Detection>(det, true);
        input_thr.join();
        det_thr.join();
        output_thr.join();
    }

}
