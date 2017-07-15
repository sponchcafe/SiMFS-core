#ifndef IO_H
#define IO_H

#include <iostream>

namespace io{

        typedef struct{
            int16_t x;
            int16_t y;
            int16_t z;
        } coordinate;

        std::ostream& write_binary(std::ostream& os, const coordinate& c);
        std::istream& read_binary(std::istream &is, coordinate& c);

        typedef double timetag;
        typedef double flux;
        typedef double efficiency;

        std::ostream& write_binary(std::ostream& os, const double& x);
        std::istream& read_binary(std::istream &is, double& x);

}

#endif