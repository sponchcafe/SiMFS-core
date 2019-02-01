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

    //-Routed-timetag----------------------------------------------------//
    typedef struct{
        realtime_t time;
        unsigned short route;
    } RoutedTime;

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

}
