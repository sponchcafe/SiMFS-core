#ifndef SIM_CMDUTILS_DEFS_H
#define SIM_CMDUTILS_DEFS_H

#include <cstdint>

namespace sim{

    //-------------------------------------------------------------------//
    typedef int16_t coord_t;
    typedef double si_coord_t;
    
    typedef double realtime_t;

    typedef uint32_t photon_count_t;

    typedef struct{
        realtime_t time;
        double value;
    } TimedValue;

    //-Coordinate-in-integer-nm------------------------------------------//
    typedef struct{
        coord_t x;
        coord_t y;
        coord_t z;
    } Coordinate;

    //-SI-Coordinate-in-double-m-----------------------------------------//
    typedef struct{
        si_coord_t x;
        si_coord_t y;
        si_coord_t z;
    } SI_Coordinate;

    //-------------------------------------------------------------------//
    Coordinate coordinate_m_to_nm(SI_Coordinate const c_m){
        Coordinate c_nm = {
            (coord_t) (round(1e+9*c_m.x)),
            (coord_t) (round(1e+9*c_m.y)),
            (coord_t) (round(1e+9*c_m.z))
        };
        return c_nm;
    }
}

#endif
