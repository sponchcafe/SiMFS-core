#include <iostream>
#include "mixer/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Mixer mix{};

    //-Configure-------------------------------------------------------------//
    mix.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = mix.get_json();
    cli::log_parameters(log);


    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        auto out_thread = io::buffer2file_thread<realtime_t>(log["output"]);

        std::vector<std::string> inputs = log["inputs"];
        std::vector<std::thread> in_threads{};

        for (auto it=inputs.begin(); it!=inputs.end(); ++it){
            in_threads.emplace_back(io::file2buffer_thread<realtime_t>(*it));
        }

        auto mix_thread = comp::run_component<comp::Mixer> (mix, true);
        mix_thread.join();
        out_thread.join();
        for (auto &th: in_threads) th.join();

    }

}
