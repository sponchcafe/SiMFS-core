#include "shift/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"
#include <thread>

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Shifter sft;

    //-Configure-------------------------------------------------------------//
    sft.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = sft.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        //-Threads-----------------------------------------------------------//
        std::thread in_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        std::thread out_thr = io::buffer2file_thread<Coordinate>(log["output"]);
        std::thread sft_thr= comp::run_component<comp::Shifter>(sft, true);
        //-------------------------------------------------------------------//

        sft_thr.join();
        in_thr.join();
        out_thr.join();

    }

}
