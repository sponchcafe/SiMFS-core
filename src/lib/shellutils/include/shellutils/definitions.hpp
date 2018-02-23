#ifndef SIM_CMDUTILS_DEFS_H
#define SIM_CMDUTILS_DEFS_H

#include <cstdint>

namespace sim{

    //-------------------------------------------------------------------//
    typedef int16_t coord_nm_t;
    typedef double coord_t;
    
    typedef double realtime_t;
    typedef uint32_t photon_count_t;

    typedef struct{
        realtime_t time;
        double value;
    } TimedValue;

    //-Coordinate-in-integer-nm------------------------------------------//
    typedef struct{
        coord_nm_t x;
        coord_nm_t y;
        coord_nm_t z;
    } Coordinate_nm;

    //-SI-Coordinate-in-double-m-----------------------------------------//
    typedef struct{
        coord_t x;
        coord_t y;
        coord_t z;
        realtime_t t;
    } Coordinate;

    //-------------------------------------------------------------------//
    Coordinate_nm coordinate_m_to_nm(Coordinate const c){
        Coordinate_nm c_nm = {
            (coord_nm_t) (round(1e+9*c.x)),
            (coord_nm_t) (round(1e+9*c.y)),
            (coord_nm_t) (round(1e+9*c.z))
        };
        return c_nm;
    }
}

#endif
