#include <iostream>
#include "diffusion/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"
#include "io/queue_io.hpp"
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

    //-Initialize------------------------------------------------------------//
    dif.init();

    //-Log-------------------------------------------------------------------//
    json log = dif.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        //-Threads-----------------------------------------------------------//
        std::thread coord_thr = queue_io::queue_to_file_thread<Coordinate>(log["coordinate_output"]);
        std::thread collision_thr = queue_io::queue_to_file_thread<Coordinate>(log["collision_output"]);
        std::thread dif_thr = comp::run_component<comp::Diffusion>(dif);
        //-------------------------------------------------------------------//

        dif_thr.join();
        collision_thr.join();
        coord_thr.join();

    }

}
