#pragma once
#include <cstddef>
#include <iostream>

namespace sim{
    namespace grid{

        typedef struct {
            double x;
            double y;
            double z;
        } Coordinate;

        //-------------------------------------------------------------------//
        typedef struct {
            double min;
            double max;
            size_t n;
        } LinSpace;

        //-------------------------------------------------------------------//
        typedef struct {
            LinSpace x;
            LinSpace y;
            LinSpace z;
        } GridSpace;

        //-------------------------------------------------------------------//
        Coordinate operator-(Coordinate const &c);
        Coordinate operator-(Coordinate const &lhs, Coordinate const &rhs);
        Coordinate operator+(Coordinate const &lhs, Coordinate const &rhs);
        Coordinate operator*(Coordinate const &lhs, double rhs);

        //-------------------------------------------------------------------//
        bool operator==(Coordinate const &lhs, Coordinate const &rhs);
        bool operator==(LinSpace const &lhs, LinSpace const &rhs);
        bool operator==(GridSpace const &lhs, GridSpace const &rhs);
        bool operator!=(GridSpace const &lhs, GridSpace const &rhs);

        //-------------------------------------------------------------------//
        std::ostream &operator<<(std::ostream &os, Coordinate const &coord);

    }
}
