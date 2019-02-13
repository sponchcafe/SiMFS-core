#include "excitation/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;

//---------------------------------------------------------------------------//
int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Excitation exi{};

    //-Log-------------------------------------------------------------------//
    exi.set_json(params);
    json log = exi.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto exi_thr = comp::run_component<comp::Excitation>(exi, true);
        input_thr.join();
        exi_thr.join();
        output_thr.join();
    }

}
