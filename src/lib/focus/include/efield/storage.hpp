#pragma once

#include <string>
#include <vector>
#include "efield/efield.hpp"
#include "efield/types.hpp"

namespace sim{
    namespace field{

        //-------------------------------------------------------------------//
        void load_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &t);
        void save_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &s);
        void make_grid_coordinates(EFieldSpec spec, std::vector<EFieldCoordinate> &t);
        void evaluate_efield_grid(focus::EField &f, std::vector<EFieldCoordinate> &c, std::vector<EFieldComponents> &t);

    }
}
