#include "component/component.hpp"
#include "io/buffer.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class Shifter : public Component{

            public:

                //-----------------------------------------------------------//
                Shifter ();

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
                void set_shift(double x, double y, double z);
                void set_delay(double t);
                void set_input_id(std::string id);
                void set_output_id(std::string id);
                //-----------------------------------------------------------//
               

            private:

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//
                double shift_x = 0.0;
                double shift_y = 0.0;
                double shift_z = 0.0;
                double delay_t = 0.0;
                std::string input_id = "__coordinates__";
                std::string output_id = "__shifted_coordinates__";

                //-----------------------------------------------------------//
                std::unique_ptr<io::BufferInput<Coordinate>> input_ptr;
                std::unique_ptr<io::BufferOutput<Coordinate>> output_ptr;

        };

    }
}
