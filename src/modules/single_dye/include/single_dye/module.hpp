#pragma once

#include "diffusion/component.hpp"
#include "excitation/component.hpp"
#include "detection/component.hpp"
#include "photophysics/component.hpp"
#include "splitter/component.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class SingleDye : public Component{

            public:

                //-----------------------------------------------------------//
                SingleDye ();

                //-----------------------------------------------------------//
                // Component interface 
                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;
                //-----------------------------------------------------------//

               
            private:

                //-----------------------------------------------------------//
                void run_dif(std::unique_ptr<comp::Diffusion> dif);
                void run_exi(std::unique_ptr<comp::Excitation> exi);
                void run_det(std::unique_ptr<comp::Detection> det);
                void run_ph2(std::unique_ptr<comp::Photophysics> ph2);
                void run_spl(std::unique_ptr<comp::Splitter> spl);
                void run_buffer();

                //-----------------------------------------------------------//
                json get_reduced_dif_params();
                json get_reduced_exi_params();
                json get_reduced_det_params();
                json get_reduced_ph2_params();
                json get_reduced_spl_params();

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                json dif_params;
                json exi_params;
                json det_params;
                json ph2_params;
                std::string photon_output_id = "photons.dat";

                //-----------------------------------------------------------//
                std::unique_ptr<comp::Diffusion> dif;
                std::unique_ptr<comp::Excitation> exi;
                std::unique_ptr<comp::Detection> det;
                std::unique_ptr<comp::Photophysics> ph2;
                std::unique_ptr<comp::Splitter> spl;

        };

    }
}
