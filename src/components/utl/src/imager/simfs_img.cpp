#include <iostream>
#include "imager/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Imager img{};

    //-Configure-------------------------------------------------------------//
    img.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = img.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        std::vector<std::thread> threads{};
        std::vector<std::string> coord_inputs = log["coordinate_inputs"];
        std::vector<std::string> time_inputs = log["time_inputs"];
        
        for (auto it=coord_inputs.begin(); it!=coord_inputs.end(); ++it){
            threads.emplace_back(io::file2buffer_thread<Coordinate>(*it));
        }
        for (auto it=time_inputs.begin(); it!=time_inputs.end(); ++it){
            threads.emplace_back(io::file2buffer_thread<realtime_t>(*it));
        }

        std::thread img_thr = comp::run_component<comp::Imager>(img, true);
        for (auto &th: threads) th.join();
        img_thr.join();

    }

}
