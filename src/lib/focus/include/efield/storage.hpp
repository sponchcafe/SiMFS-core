#pragma once

#include <string>
#include <vector>
#include <atomic>
#include "efield/efield.hpp"
#include "efield/types.hpp"

namespace sim{
    namespace field{

        //-------------------------------------------------------------------//
        bool check_file_exists(std::string fname);

        //-------------------------------------------------------------------//
        void load_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &t);
        void save_field_file(std::string fname, EFieldSpec header, std::vector<EFieldComponents> &s);

        //-------------------------------------------------------------------//
        void make_grid_coordinates(EFieldSpec spec, std::vector<EFieldCoordinate> &t);
        void evaluate_prefactors(
                EField &f, 
                EFieldSpec &spec
                );
        void evaluate_efield_grid(
                EField &f, 
                std::vector<EFieldCoordinate> &c, 
                std::vector<EFieldComponents> &t);
        void evaluate_efield_grid_section(
                EField &f, 
                std::atomic<size_t> &prg,
                std::vector<EFieldCoordinate>::iterator c_begin,
                std::vector<EFieldCoordinate>::iterator c_end,
                std::vector<EFieldComponents>::iterator f_begin);
    }

}
