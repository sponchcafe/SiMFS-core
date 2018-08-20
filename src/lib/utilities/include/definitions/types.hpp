#pragma once
#include <complex>
#include <functional>

namespace sim{

    //-Meters------------------------------------------------------------//
    typedef double coordinate_t;
    
    //-Seconds-----------------------------------------------------------//
    typedef double realtime_t;

    //-Timed-double-value------------------------------------------------//
    typedef struct{
        double value;
        realtime_t time;
    } TimedValue;

    //-SI-Coordinate-----------------------------------------------------//
    typedef struct{
        coordinate_t x;
        coordinate_t y;
        coordinate_t z;
        realtime_t t;
    } Coordinate;

    //-Enums-------------------------------------------------------------//
    enum class FocusMode {EXCITATION, DETECTION};
    enum class FocusType {GAUSS, ALPHA, EFIELD};
    enum class Interpolation {NEAREST, LINEAR};

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
    struct EFieldGridSpec {
        EFieldDimension x;
        EFieldDimension y;
        EFieldDimension z;
    };

}
