#include "pulse/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"
#include <thread>

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Pulse pls{};

    //-Configure-------------------------------------------------------------//
    pls.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = pls.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        //-Threads-----------------------------------------------------------//
        std::thread input_thr = io::file2buffer_thread<TimedValue>(log["input"]);
        std::thread output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        std::thread pls_thr = comp::run_component<comp::Pulse>(pls, true);
        //-------------------------------------------------------------------//

        pls_thr.join();
        input_thr.join();
        output_thr.join();

    }

}
