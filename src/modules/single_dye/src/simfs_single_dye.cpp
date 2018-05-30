#include <iostream>
#include <thread>

#include "diffusion/component.hpp"
#include "excitation/component.hpp"
#include "detection/component.hpp"
#include "buffer/component.hpp"
#include "photophysics/component.hpp"
#include "splitter/component.hpp"

#include "component/cli.hpp"

#include "io/file_io.hpp"
#include "io/queue_io.hpp"

using namespace sim;

void run_dif (std::unique_ptr<comp::Diffusion> dif){
    dif->set_coordinate_output< queue_io::QueueOutput >("__coords__");
    dif->set_collision_output<  file_io::FileOutput >(); 
    dif->run();
}
void run_exi (std::unique_ptr<comp::Excitation> exi){
    exi->set_coordinate_input< queue_io::QueueInput >("__coords_exi__");
    exi->set_flux_output< queue_io::QueueOutput >("__flux__");
    exi->run();
}
void run_det (std::unique_ptr<comp::Detection> det){
    det->set_coordinate_input< queue_io::QueueInput >("__coords_det__");
    det->set_efficiency_output< queue_io::QueueOutput >("__efficiency__");
    det->run();
}
void run_ph2 (std::unique_ptr<comp::Photophysics> ph2){
    graph::ExcitationAction &exi_act = ph2->get_action<graph::ExcitationAction>("excitation");
    graph::EmissionAction &emi_act = ph2->get_action<graph::EmissionAction>("emission");
    exi_act.set_flux_input < queue_io::QueueInput > ("__flux__");
    emi_act.set_photon_output < queue_io::QueueOutput > ("__emission__");
    ph2->run();
}
void run_spl (std::unique_ptr<comp::Splitter> spl){
    spl->set_photon_input < queue_io::QueueInput > ("__emission__");
    spl->set_efficiency_input < queue_io::QueueInput > ("__efficiency__");
    spl->set_accepted_photon_output < file_io::FileOutput > ();
    spl->set_rejected_photon_output < file_io::FileOutput > ();
    spl->run();
}
void run_buf (){
    queue_io::QueueInput< Coordinate > coords_in{"__coords__"};
    queue_io::QueueOutput< Coordinate > coords_exi{"__coords_exi__"};
    queue_io::QueueOutput< Coordinate > coords_det{"__coords_det__"};
    Coordinate c;
    while (coords_in.get(c)){
        coords_exi.put(c);
        coords_det.put(c);
    }
}


int main(int argc, char *argv[]) {

    //-Get-parameters--------------------------------------------------------//
    json params = cli::get_parameters();
    std::vector<std::string> opts = cli::parse_argv_vector(argc, argv);

    //-Create----------------------------------------------------------------//
    auto dif = std::make_unique<comp::Diffusion>();
    auto exi = std::make_unique<comp::Excitation>();
    auto det = std::make_unique<comp::Detection>();
    auto ph2 = std::make_unique<comp::Photophysics>();
    auto spl = std::make_unique<comp::Splitter>();

    //-Configure-------------------------------------------------------------//
    try{
    dif->set_json(params.at("dif"));
    } catch (json::out_of_range &e){ }
    try{
    exi->set_json(params.at("exi"));
    } catch (json::out_of_range &e){ }
    try{
    det->set_json(params.at("det"));
    } catch (json::out_of_range &e){ }
    try{
    ph2->set_json(params.at("ph2"));
    } catch (json::out_of_range &e){ }
    try{
    spl->set_json(params.at("spl"));
    } catch (json::out_of_range &e){ }
    
    ph2->init();
    dif->init();
    exi->init();
    det->init();
    spl->init();


    //-Log-------------------------------------------------------------------//
    json log;
    log["dif"] = dif->get_json();
    log["exi"] = exi->get_json();
    log["det"] = det->get_json();
    log["ph2"] = ph2->get_json();
    log["spl"] = spl->get_json();
    cli::log_parameters(log);

    //-Run-------------------------------------------------------------------//
    if (!cli::check_list(opts)){

        std::thread dif_thr{[&] () {
            run_dif(std::move(dif));
        }};
        std::thread exi_thr{[&] () {
            run_exi(std::move(exi));
        }};
        std::thread det_thr{[&] () {
            run_det(std::move(det));
        }};
        std::thread buf_thr{[&] () {
            run_buf();
        }};
        std::thread ph2_thr{[&] () {
            run_ph2(std::move(ph2));
        }};
        std::thread spl_thr{[&] () {
            run_spl(std::move(spl));
        }};

        exi_thr.join();
        dif_thr.join();
        buf_thr.join();
        det_thr.join();
        ph2_thr.join();
        spl_thr.join();

    }

}
