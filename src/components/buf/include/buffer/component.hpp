#include "component/component.hpp"
#include <thread>
#include <vector>

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Buffer : public Component{

            public:

                //-----------------------------------------------------------//
                Buffer ();

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


                //-----------------------------------------------------------//
                // Parameter setters
                //-----------------------------------------------------------//
                void set_input_id(std::string id);
                void set_output_ids(std::vector<std::string> ids);
                //-----------------------------------------------------------//
               
                
                //-----------------------------------------------------------//
                // Function templates for setting in- and outputs
                //-----------------------------------------------------------//
                template <template <class> class OutputT>
                void set_outputs(){
                    set_outputs<OutputT>(output_ids);
                }

                //-----------------------------------------------------------//
                template <template <class> class OutputT> 
                void set_outputs(std::vector<std::string> ids){
                    output_ids = ids;
                    output_ptrs.clear();
                    for (auto &it: output_ids){
                        std::unique_ptr<Output<char>> output_ptr = 
                            create_output<OutputT, char>(it);
                        output_ptrs.push_back(std::move(output_ptr));
                    }
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_input(){
                    set_input<InputT>(input_id);
                }

                //-----------------------------------------------------------//
                template <template <class> class InputT>
                void set_input(std::string id){
                    input_id = id;
                    input_ptr = create_input<InputT, char> (input_id);
                } 
                //-----------------------------------------------------------//
                
                //-----------------------------------------------------------//

            private:

                //-----------------------------------------------------------//
                void output_worker(std::string id, std::unique_ptr<Output<char>> &out);
                void input_worker();
                
                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                std::string input_id = "./buffer_input";
                std::vector<std::string> output_ids{"./buffer_output"};

                std::unique_ptr<Input<char>> input_ptr;
                std::vector<std::unique_ptr<Output<char>>> output_ptrs;


        };

    }
}
