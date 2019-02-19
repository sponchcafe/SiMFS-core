#include "component/cli.hpp"
#include "precalculate/component.hpp"
#include "shape/main.hpp"

using namespace sim;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    comp::Precalculator pre{};

    //-Log-------------------------------------------------------------------//
    pre.set_json(params);
    json log = pre.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        pre.init();
        pre.run();
    }


}
