#include "component/component.hpp"
#include "component/types.hpp"
#include "focus/base.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Excitation : public Component{

            public:

                //-----------------------------------------------------------//
                Excitation ();

                //-----------------------------------------------------------//
                void set_json(json j) override;

                //-----------------------------------------------------------//
                json get_json() override;

                //-----------------------------------------------------------//
                void init() override;

                //-----------------------------------------------------------//
                void run() override;

                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_wavelength(double w);
                void set_power(double p);
                void set_coordinate_input_id(std::string id);
                void set_flux_output_id(std::string id);
                void set_pulse(double rep, std::vector<double> &t, std::vector<double> &a);
                void set_focus(json params);
                //-----------------------------------------------------------//
                 
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_coordinate_input(){
                    set_coordinate_input<InputT>(coordinate_input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT> 
                void set_coordinate_input(std::string id){
                    set_coordinate_input_id(id);
                    coordinate_input_ptr = 
                        create_input<InputT, Coordinate>(
                            coordinate_input_id
                        );
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_flux_output(){
                    set_flux_output<OutputT>(flux_output_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_flux_output(std::string id){
                    set_flux_output_id(id);
                    flux_output_ptr = 
                    create_output<OutputT, TimedValue>(
                            flux_output_id
                        );
                }
                //-----------------------------------------------------------//
                
            private:

                //-----------------------------------------------------------//
                void normalize_pulse();
                void run_cw();
                void run_pulsed();

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double wavelength = 488e-9;                             // m
                double power = 10e-6;                                   // W
                double repetition_time = 1e-7;                          // s
                std::vector<double> pulse_time{0.0};                    // s
                std::vector<double> pulse_power{1.0};               // rel
                std::string coordinate_input_id = "./coords";
                std::string flux_output_id = "./excitation";
                //-----------------------------------------------------------//

                //-----------------------------------------------------------//
                bool cw = false; // if no pulse is specified, cw ex. is assumed
                Coordinate c{0.0,0.0,0.0,0.0};
                TimedValue flux{0.0, 0.0};
                               
                //-----------------------------------------------------------//
                std::unique_ptr<focus::Focus> focus_ptr;
                std::unique_ptr<Input<Coordinate>> coordinate_input_ptr;
                std::unique_ptr<Output<TimedValue>> flux_output_ptr;

        };
    }
}
