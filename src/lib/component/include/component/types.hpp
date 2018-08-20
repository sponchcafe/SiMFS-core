#pragma once
#include "json/json.hpp"
#include <functional>

using json = nlohmann::json;

namespace sim{

    double const CONST_E = 2.7182818284590452353602874713527;
    double const CONST_C = 299792458.0;
    double const CONST_H = 6.62607004e-34;
    double const CONST_NA = 6.022140857e+23;
    double const CONST_PI = 3.141592653589793;

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

    using json_filter_t = std::function<bool(json const &)>;


}
