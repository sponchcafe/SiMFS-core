#pragma once

#include <complex>
#include <iostream>
#include <string>


namespace sim{
    namespace field{

        //-------------------------------------------------------------------//
        enum class Interpolation{NEAREST, LINEAR};
        Interpolation interpolation_from_str(std::string s){
            if (s == "nearest") return Interpolation::NEAREST;
            else if (s == "linear") return Interpolation::LINEAR;
            else{
                std::cerr << "Interpolation " << s << " not understood\n";
                return Interpolation::NEAREST;
            }
        }
        std::string interpolation_to_str(Interpolation type){
            switch(type){
                case Interpolation::NEAREST:
                    return "nearest";
                case Interpolation::LINEAR:
                    return "linear";
                default:
                    return "nearest";
            }
        }

        //-------------------------------------------------------------------//
        struct EFieldCoordinate{
            double x;
            double y;
            double z;
        };

        //-------------------------------------------------------------------//
        struct EFieldComponents{
            std::complex<double> x;
            std::complex<double> y;
            std::complex<double> z;
        };

        //-------------------------------------------------------------------//
        struct EFieldDimension {
            double min;
            double max;
            size_t n;
        };

        //-------------------------------------------------------------------//
        struct EFieldSpec {
            double flux_density_prefactor;
            double efficiency_prefactor;
            EFieldDimension x;
            EFieldDimension y;
            EFieldDimension z;
        };



    }

}
