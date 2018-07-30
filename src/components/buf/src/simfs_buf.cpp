#include <iostream>
#include "buffer/component.hpp"
#include "component/cli.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Buffer buf{};

    //-Configure-------------------------------------------------------------//
    buf.set_json(params);

    //-Log-------------------------------------------------------------------//
    json log = buf.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        auto buf_thread = comp::run_component<comp::Buffer>(buf);
        auto in_thread = io::file2buffer_thread<char>(log["input"]);
        
        std::vector<std::thread> out_threads{};
        std::vector<std::string> outputs = log["outputs"];

        for (auto it=outputs.begin(); it!=outputs.end(); ++it){
            out_threads.emplace_back(io::buffer2file_thread<char>(*it));
        }

        in_thread.join();
        buf_thread.join();
        for (auto &th: out_threads) th.join();

    }

}
