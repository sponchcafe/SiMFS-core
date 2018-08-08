#pragma once
#include "json/json.hpp"
#include <functional>

using json = nlohmann::json;

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

    using json_filter_t = std::function<bool(json const &)>;


}
