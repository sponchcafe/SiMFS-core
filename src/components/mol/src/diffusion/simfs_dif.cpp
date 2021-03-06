#include <iostream>
#include "diffusion/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"
#include <thread>

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Diffusion dif;

    //-Configure-------------------------------------------------------------//
    dif.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = dif.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        //-Init--------------------------------------------------------------//
        dif.init();

        //-Threads-----------------------------------------------------------//
        std::thread coord_thr = io::buffer2file_thread<Coordinate>(log["coordinate_output"]);
        std::thread collision_thr = io::buffer2file_thread<realtime_t>(log["collision_output"]);
        std::thread dif_thr = comp::run_component<comp::Diffusion>(dif);
        //-------------------------------------------------------------------//

        dif_thr.join();
        collision_thr.join();
        coord_thr.join();

    }

}
