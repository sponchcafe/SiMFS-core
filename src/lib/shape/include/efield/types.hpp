#pragma once

#include <complex>
#include <iostream>
#include <string>


namespace sim{
    namespace field{


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
            double wavelength;
            double flux_density_prefactor;
            double efficiency_prefactor;
            EFieldDimension x;
            EFieldDimension y;
            EFieldDimension z;
        };



    }

}
