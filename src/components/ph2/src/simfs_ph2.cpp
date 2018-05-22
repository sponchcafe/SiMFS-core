#include <iostream>
#include "photophysics/component.hpp"
#include "component/cli.hpp"
#include "io/file_io.hpp"

using namespace sim;
using namespace sim::graph;

int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    comp::Photophysics ph2;

    //-Configure-------------------------------------------------------------//
    ph2.set_json(params);

    //-Initialize------------------------------------------------------------//
    ph2.init();

    //-Cnnect-action-io------------------------------------------------------//
    json actions = ph2.get_json()["actions"];
    for (json::iterator it=actions.begin(); it!=actions.end(); ++it){

        std::string action_type = it.value()["type"];
        std::string action_name = it.key();

        if (action_type_compare(action_type, ExcitationAction::type)){
            ExcitationAction &exi_act = 
                ph2.get_action<ExcitationAction>(action_name);
            exi_act.set_flux_input< file_io::FileInput > (); // template spec
        }

        else if (action_type_compare(action_type, EmissionAction::type)){
            EmissionAction &emi_act = 
                ph2.get_action<EmissionAction>(action_name);
            emi_act.set_photon_output< file_io::FileOutput > (); // template spec
        }

        else if (action_type_compare(action_type, TransferAction::type)){
            TransferAction &trs_act = 
                ph2.get_action<TransferAction>(action_name);
            trs_act.set_photon_input< file_io::FileInput > (); // template spec
            trs_act.set_rejected_photon_output< file_io::FileOutput > (); // template spec
        }

    }

    //-Log-------------------------------------------------------------------//
    cli::log_parameters(ph2.get_json());

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){
        ph2.run();
    }

}
