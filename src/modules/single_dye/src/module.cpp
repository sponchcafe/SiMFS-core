#include "single_dye/module.hpp"
#include "io/file_io.hpp"
#include "io/queue_io.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        SingleDye::SingleDye(){ 
            dif = std::make_unique<comp::Diffusion>();
            exi = std::make_unique<comp::Excitation>();
            det = std::make_unique<comp::Detection>();
            ph2 = std::make_unique<comp::Photophysics>();
            spl = std::make_unique<comp::Splitter>();
        }

        //-------------------------------------------------------------------//
        void SingleDye::set_photon_output_id(std::string id){
            photon_output_id = id;
        }
        //-------------------------------------------------------------------//
        
        //-------------------------------------------------------------------//
        void SingleDye::set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            dif->set_json(params["dif"]);
            exi->set_json(params["exi"]);
            det->set_json(params["det"]);
            ph2->set_json(params["ph2"]);
            dif->set_json(params["spl"]);

        }

        //-------------------------------------------------------------------//
        json SingleDye::get_json(){

            json j;

            j["dif"] = get_reduced_dif_params();
            j["exi"] = get_reduced_exi_params();
            j["det"] = get_reduced_det_params();
            j["ph2"] = get_reduced_ph2_params();
            j["spl"] = get_reduced_spl_params();

            j["photon_output"] = photon_output_id;

            return j;

        }


        //-------------------------------------------------------------------//
        void SingleDye::init() {
            ph2->init();
            dif->init();
            exi->init();
            det->init();
            spl->init();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run(){

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
                run_buffer();
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

        //-------------------------------------------------------------------//
        json SingleDye::get_reduced_dif_params(){
            json j = dif->get_json();
            j.erase("coordinate_output");
            j.erase("collision_output");
            return j;
        }

        //-------------------------------------------------------------------//
        json SingleDye::get_reduced_exi_params(){
            json j = exi->get_json();
            j.erase("coordinate_input");
            j.erase("flux_output");
            return j;
        }

        //-------------------------------------------------------------------//
        json SingleDye::get_reduced_det_params(){
            json j = det->get_json();
            j.erase("coordinate_input");
            j.erase("efficiency_output");
            return j;
        }

        //-------------------------------------------------------------------//
        json SingleDye::get_reduced_ph2_params(){
            json j = ph2->get_json();
            j["actions"]["emission"].erase("photon_output");
            j["actions"]["excitation"].erase("flux_input");
            return j;
        }

        //-------------------------------------------------------------------//
        json SingleDye::get_reduced_spl_params(){
            json j = spl->get_json();
            set_photon_output_id(j["accepted_output"]);
            j.erase("accepted_output");
            j.erase("rejected_output");
            j.erase("efficiency_input");
            j.erase("photon_input");
            return j;
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_dif(std::unique_ptr<comp::Diffusion> dif){
            dif->set_coordinate_output< queue_io::QueueOutput >("__coords__");
            dif->set_collision_output< file_io::FileOutput >("/dev/null");
            dif->run();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_exi(std::unique_ptr<comp::Excitation> exi){
            exi->set_coordinate_input< queue_io::QueueInput >("__coords_exi__");
            exi->set_flux_output< queue_io::QueueOutput >("__flux__");
            exi->run();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_det(std::unique_ptr<comp::Detection> det){
            det->set_coordinate_input< queue_io::QueueInput >("__coords_det__");
            det->set_efficiency_output< queue_io::QueueOutput >("__efficiency__");
            det->run();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_ph2(std::unique_ptr<comp::Photophysics> ph2){
            graph::ExcitationAction &exi_act = ph2->get_action<graph::ExcitationAction>("excitation");
            graph::EmissionAction &emi_act = ph2->get_action<graph::EmissionAction>("emission");
            exi_act.set_flux_input < queue_io::QueueInput > ("__flux__");
            emi_act.set_photon_output < queue_io::QueueOutput > ("__emission__");
            ph2->run();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_spl(std::unique_ptr<comp::Splitter> spl){
            spl->set_photon_input < queue_io::QueueInput > ("__emission__");
            spl->set_efficiency_input < queue_io::QueueInput > ("__efficiency__");
            spl->set_accepted_photon_output < file_io::FileOutput > ();
            spl->set_rejected_photon_output < file_io::FileOutput > ("/dev/null");
            spl->run();
        }

        //-------------------------------------------------------------------//
        void SingleDye::run_buffer(){
            queue_io::QueueInput< Coordinate > coords_in{"__coords__"};
            queue_io::QueueOutput< Coordinate > coords_exi{"__coords_exi__"};
            queue_io::QueueOutput< Coordinate > coords_det{"__coords_det__"};
            Coordinate c;
            while (coords_in.get(c)){
                coords_exi.put(c);
                coords_det.put(c);
            }
        }
    }
}
