#ifndef SIM_CONST_H
#define SIM_CONST_H


#define CONST_E 2.7182818284590452353602874713527
#define CONST_C 299792458.0
#define CONST_H 6.62607004e-34
#define CONST_NA 6.022140857e+23
#define CONST_PI 3.141592653589793

namespace sim{

    namespace io{
        
        typedef int16_t coord_t;
        typedef double si_coord_t;

        /* Coordinate in integer nm */
        typedef struct{
            coord_t x;
            coord_t y;
            coord_t z;
        } Coordinate;

        /* SI Coordinate in m */
        typedef struct SI_Coordinate{
            si_coord_t x;
            si_coord_t y;
            si_coord_t z;
            SI_Coordinate() :
                x(0), y(0), z(0) {}
            SI_Coordinate(si_coord_t x, si_coord_t y, si_coord_t z) :
                x(x), y(y), z(z) {}
            SI_Coordinate(Coordinate const &c) :
                x((si_coord_t) c.x * 1e-9),
                y((si_coord_t) c.y * 1e-9), 
                z((si_coord_t) c.z * 1e-9)
            {}
            SI_Coordinate &operator=(const Coordinate &c){
                x = (si_coord_t) c.x * 1e-9;
                y = (si_coord_t) c.y * 1e-9;
                z = (si_coord_t) c.z * 1e-9;
                return *this;
            }
        } SI_Coordinate;

        typedef double timetag;
        typedef double flux;
        typedef double efficiency;
        typedef uint32_t photon_count;
    }
}

#endif
