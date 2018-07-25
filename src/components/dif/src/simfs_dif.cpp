#include <iostream>
#include "diffusion/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"
#include "io/queue_io.hpp"
#include <thread>

using namespace sim;

void run (comp::Diffusion dif){
    dif.run();
}

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Diffusion dif;

    //-Configure-------------------------------------------------------------//
    dif.set_json(params);
    dif.set_coordinate_output<queue_io::QueueOutput>();
    dif.set_collision_output<queue_io::QueueOutput>();

    //-Initialize------------------------------------------------------------//
    dif.init();

    //-Log-------------------------------------------------------------------//
    json log = dif.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        //-File-writer-threads-----------------------------------------------//
        std::thread coord_thr{[&] () {
            queue_io::queue_to_file<Coordinate>(log["coordinate_output"]);
        }};
      
        std::thread collision_thr{[&] () {
            queue_io::queue_to_file<Coordinate>(log["collision_output"]);
        }};

        std::thread run_thr{[&] () {
            run(std::move(dif));
        }};

        run_thr.join();
        collision_thr.join();
        coord_thr.join();

    }

}
