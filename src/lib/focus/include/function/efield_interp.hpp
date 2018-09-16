#pragma once

#include "function/base.hpp"
#include "efield/storage.hpp"


namespace sim{
    namespace focus{

        using namespace field;

        //-------------------------------------------------------------------//
        enum class Interpolation{NEAREST, LINEAR};
        Interpolation interpolation_from_str(std::string s);
        std::string interpolation_to_str(Interpolation type);


        //-------------------------------------------------------------------//
        class FieldInterpolator : public Focus{

            public:

                //-----------------------------------------------------------//
                FieldInterpolator();

                //-----------------------------------------------------------//
                void set_field_file(std::string fname);
                void set_interpolation(Interpolation type);
                void set_rotation(double x, double y, double z);

                //-----------------------------------------------------------//
                void set_json(json j) override;
                json get_json() override;

                //-----------------------------------------------------------//
                double evaluate(double x, double y, double z) const override;
                double get_flux_density_prefactor() const override;
                double get_efficiency_prefactor() const override;

            private:

                //-----------------------------------------------------------//
                size_t largest_lower_index(double x, double y, double z) const;
                EFieldComponents interp_nearest(double x, double y, double z) const;
                EFieldComponents interp_linear(double x, double y, double z) const;

                //-----------------------------------------------------------//
                double rot_x = 1.0;
                double rot_y = 1.0;
                double rot_z = 1.0;

                //-----------------------------------------------------------//
                std::vector<EFieldComponents> field;
                EFieldSpec spec;

                //-----------------------------------------------------------//
                double x_step;
                double y_step;
                double z_step;

                //-----------------------------------------------------------//
                size_t x_inc;
                size_t y_inc;
                size_t z_inc;

                std::string field_file_name = "__field__.dat";
                Interpolation interpolation = Interpolation::NEAREST;

        };

    }
}
