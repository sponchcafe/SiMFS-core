#include "definitions/types.hpp"
#include "grid/grid.hpp"

namespace sim{
    namespace grid{

        //-------------------------------------------------------------------//
        Coordinate operator-(Coordinate const &c){
            Coordinate res{};
            res.x = -c.x;
            res.y = -c.y;
            res.z = -c.z;
            return res;
        }

        //-------------------------------------------------------------------//
        Coordinate operator-(Coordinate const &lhs, Coordinate const &rhs){
            return lhs + (-rhs);
        }

        //-------------------------------------------------------------------//
        Coordinate operator+(Coordinate const &lhs, Coordinate const &rhs){
            Coordinate res{};
            res.x = lhs.x + rhs.x;
            res.y = lhs.y + rhs.y;
            res.z = lhs.z + rhs.z;
            return res;
        }

        //-------------------------------------------------------------------//
        Coordinate operator*(Coordinate const &lhs, double rhs){
            Coordinate res{};
            res.x = rhs*lhs.x;
            res.y = rhs*lhs.y;
            res.z = rhs*lhs.z;
            return res;
        }

        //-------------------------------------------------------------------//
        bool operator==(Coordinate const &lhs, Coordinate const &rhs){
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        bool operator==(LinSpace const &lhs, LinSpace const &rhs){
            return lhs.min == rhs.min && lhs.max == rhs.max && lhs.n == rhs.n;
        }

        bool operator==(GridSpace const &lhs, GridSpace const &rhs){
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
        }

        bool operator!=(GridSpace const &lhs, GridSpace const &rhs){
            return !(lhs == rhs);
        }

        //-------------------------------------------------------------------//
        std::ostream &operator<<(std::ostream &os, Coordinate const &coord){
            os << "<Coordinate ";
            os << "x=" << coord.x << ", ";
            os << "y=" << coord.y << ", ";
            os << "z=" << coord.z << ">";
            return os;
        }

    }
}
