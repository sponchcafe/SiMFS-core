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

        //-------------------------------------------------------------------//
        void to_json(json& j, const LinSpace& l){
            j = json{{"min", l.min}, {"max", l.max}, {"n", l.n}};
        }
        
        void from_json(const json& j, LinSpace& l){
            j.at("min").get_to(l.min);
            j.at("max").get_to(l.max);
            j.at("n").get_to(l.n);
        }

        void to_json(json& j, const GridSpace& g){
                to_json(j["x"], g.x);
                to_json(j["y"], g.y);
                to_json(j["z"], g.z);
        }

        void from_json(const json& j, GridSpace& g){
            from_json(j["x"], g.x);
            from_json(j["y"], g.y);
            from_json(j["z"], g.z);
        }
        //-------------------------------------------------------------------//


    }
}
