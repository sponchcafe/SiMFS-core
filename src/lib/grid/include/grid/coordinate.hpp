#pragma once
#include <cstddef>
#include <iostream>
#include "json/json.hpp"

using json = nlohmann::json;

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

        void to_json(json& j, const LinSpace& l);
        void from_json(const json& j, LinSpace& l);

        void to_json(json& j, const GridSpace& g);
        void from_json(const json& j, GridSpace& g);

    }
}
