#include <iostream>
#include "gauss/component.hpp"
#include "alpha/component.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"
#include "pulse/component.hpp"

using namespace sim;

struct Mode{
    FocusMode mode;
    FocusType type;
};

//--------------------------------------------------------------------------//
Mode get_mode(std::vector<std::string> opts){

    Mode m;
    
    if (cli::search_token(opts, "gauss")) m.type = FocusType::GAUSS;
    else if (cli::search_token(opts, "alpha")) m.type = FocusType::ALPHA;
    else if (cli::search_token(opts, "efield")) m.type = FocusType::EFIELD;
    else m.type = FocusType::GAUSS;

    if(cli::search_token(opts, "det")) m.mode = FocusMode::DETECTION;
    else if (cli::search_token(opts, "exi")) m.mode = FocusMode::EXCITATION;
    else m.mode = FocusMode::EXCITATION;

    return m;

}

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    // comp::FCS_Gauss fcs;
    
    Mode mode = get_mode(opts);

    switch (mode.type) {
        case FocusType::GAUSS : 
            std::cerr << "Using Gaussian focus funcion\n";
            break;
        case FocusType::ALPHA :
            std::cerr << "Using Alpha flux focus funcion\n";
            break;
        case FocusType::EFIELD :
            std::cerr << "Using E-field focus function\n";
            break;
    }

    switch (mode.mode) {
        case FocusMode::EXCITATION :
            std::cerr << "Excitation mode\n";
            break;
        case FocusMode::DETECTION :
            std::cerr << "Detection mode\n";
            break;
    }
    
//-Configure-------------------------------------------------------------//

    //-Log-------------------------------------------------------------------//
    /*
    fcs.set_json(params);
    json log = fcs.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto exi_thr = comp::run_component<comp::FCS_Gauss>(fcs, true);
        input_thr.join();
        exi_thr.join();
        output_thr.join();
    }
    */


}
