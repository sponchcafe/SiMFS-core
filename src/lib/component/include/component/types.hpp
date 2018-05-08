#ifndef SIM_TYPES_H
#define SIM_TYPES_H

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
        coord_t x;
        coord_t y;
        coord_t z;
        realtime_t t;
    } Coordinate;

}
#endif
