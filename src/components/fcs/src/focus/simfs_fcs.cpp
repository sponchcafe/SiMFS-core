#include <iostream>
#include "focus/component.hpp"
#include "function/gauss.hpp"
#include "function/alpha.hpp"
#include "function/efield_interp.hpp"
#include "component/cli.hpp"
#include "io/buffer.hpp"

using namespace sim;

//--------------------------------------------------------------------------//
struct Mode{ FocusMode mode; FocusType type; };
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
//---------------------------------------------------------------------------//


//---------------------------------------------------------------------------//
int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    Mode mode = get_mode(opts);

    comp::FCS fcs{mode.mode};
    std::unique_ptr<focus::Focus> focus_function;

    switch (mode.type) {
        case FocusType::GAUSS : 
            std::cerr << "Using Gaussian focus funcion\n";
            focus_function = std::make_unique<focus::Gauss>();
            // CONFIGURE GAUSS
            break;
        case FocusType::ALPHA :
            std::cerr << "Using Alpha flux focus funcion\n";
            focus_function = std::make_unique<focus::Alpha>();
            // CONFIGURE ALPHA
            break;
        case FocusType::EFIELD :
            std::cerr << "Using E-field focus function\n";
            focus_function = std::make_unique<focus::FieldInterpolator>();
            // CONFIGURE EFIELD
            break;
    }

    fcs.set_focus_ptr(focus_function);

    //-Log-------------------------------------------------------------------//
    fcs.set_json(params);
    json log = fcs.get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        auto input_thr = io::file2buffer_thread<Coordinate>(log["input"]);
        auto output_thr = io::buffer2file_thread<TimedValue>(log["output"]);
        auto fcs_thr = comp::run_component<comp::FCS>(fcs, true);
        input_thr.join();
        fcs_thr.join();
        output_thr.join();
    }

}
