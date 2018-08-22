#include "component/component.hpp"
#include "efield/efield.hpp"

namespace sim{
    namespace comp{

        //-------------------------------------------------------------------//
        class EFieldPrecalculator : public Component{

            public:

                //-----------------------------------------------------------//
                EFieldPrecalculator ();

                //-----------------------------------------------------------//
                void set_gridspec_x(double x_min, double x_max, size_t n);
                void set_gridspec_y(double y_min, double y_max, size_t n);
                void set_gridspec_z(double z_min, double z_max, size_t n);
                void set_filename(std::string fn);

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
                field::EField field_function{};
                std::string fname = "__field__.dat";
                field::EFieldSpec spec{
                    488e-9, 1.0, 1.0, 
                        {-2.5e-6, 2.5e-6, 32},
                        {-2.5e-6, 2.5e-6, 32},
                        {-2.5e-6, 2.5e-6, 32}
                };

                //-----------------------------------------------------------//
                // Simulation parameters + defaults
                //-----------------------------------------------------------//

        };

    }
}
