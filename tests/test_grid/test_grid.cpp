#include "catch.hpp"
#include "grid/grid.hpp"
#include "grid/nearest_grid.hpp"
#include "grid/trilinear_grid.hpp"
#include "definitions/types.hpp"
#include <functional>
#include <complex>
#include <cmath>

using namespace sim;
using namespace sim::grid;

TEMPLATE_TEST_CASE ("The grid stores and returns values at coordinates", 
        "[grid][access]", unsigned, int, double, std::complex<double>) {

    auto linspace = LinSpace{-1, 1, 3};
    auto grid_space = GridSpace{linspace, linspace, linspace};
    auto grid = NearestGrid<TestType>{grid_space};

    GIVEN ("an empty grid") {

        Coordinate coord{0,0,0};

        THEN("any coordinate should return 0.0"){
            REQUIRE(grid.get(coord) == 0.0);
        }

        WHEN("a coordinate is set to a value"){

            grid.set(coord, 42.0);

            THEN("the exact coordinate should return this value"){
                REQUIRE(grid.get(coord) == 42.0);
            }

            AND_THEN("coordinates close on the grid should also return this value"){
                Coordinate coord_close_upper{0,0,0.49};
                REQUIRE(grid.get(coord_close_upper) == 42.0);
                Coordinate coord_close_lower{0,0,-0.5};
                REQUIRE(grid.get(coord_close_lower) == 42.0);
            }

            AND_THEN("coordinates values far on the grid should still return 0.0"){
                Coordinate coord_far{0,0,0.9};
                REQUIRE(grid.get(coord_far) == 0.0);
            }

        }

        WHEN("a coordinate close coordinate is set to a value"){

            Coordinate coord_close{1.2,-0.9,-1.1};
            Coordinate coord_nearest{1,-1,-1};

            grid.set(coord_close, 42.0);

            THEN("the exact (nearest) coordinate should return this value"){
                REQUIRE(grid.get(coord_nearest) == 42.0);
            }

            Coordinate coord_far{0,0,0};
            AND_THEN("coordinates far on the grid should still return 0.0"){
                REQUIRE(grid.get(coord_far) == 0.0);
            }

        }

    }

    GIVEN("a grid with the z=min plane set to a value"){

        for (double x=-1; x<=1; x+=1){
            for (double y=-1; y<=1; y+=1){
                grid.set(Coordinate{x, y, -1}, 42.0);
            }
        }

        THEN("coordinate on the z=z_min edge should return this value"){
            Coordinate coord_on_z{0,0,-1};
            REQUIRE(grid.get(coord_on_z) == 42.0);
        }

        AND_THEN("coordinates out of the z range should return this value"){
            Coordinate coord_out_z{0,0,-2};
            REQUIRE(grid.get(coord_out_z) == 42.0);
        }

    }

}


TEMPLATE_TEST_CASE ("Grid can be populated by mapping a function", 
        "[grid][map]", double, std::complex<double>) {

    auto linspace = LinSpace{-1, 1, 3};
    auto grid_space = GridSpace{linspace, linspace, linspace};
    auto grid = NearestGrid<TestType>{grid_space};

    GIVEN ("an empty grid") {

        WHEN ("a constant function is mapped to the grid"){

            std::function<TestType(Coordinate &c)> constant = [](Coordinate &c) { 
                return TestType{42.0};
            };

            grid.map(constant);

            THEN("any coordinate should return this constant value"){
                REQUIRE(grid.get(Coordinate{0,0,0}) == 42.0);
                REQUIRE(grid.get(Coordinate{1,0.4,-0.1}) == 42.0);
                REQUIRE(grid.get(Coordinate{4,-3,8}) == 42.0);
                REQUIRE(grid.get(Coordinate{1,1,1}) == 42.0);
            }
        }

        WHEN ("a delta function is mapped to the grid"){

            Coordinate target{0,0,0};

            std::function<TestType(Coordinate &c)> delta = [=](Coordinate &c) { 
                return (c == target) ? 42.0 : 0;
            };

            grid.map(delta);

            THEN("the exact coordinate should return this constant value"){
                REQUIRE(grid.get(target) == 42.0);
                AND_THEN("coordinates far from the delta peak should still return 0.0"){
                    REQUIRE(grid.get(Coordinate{1,1,1}) == 0.0);
                }
           }

        }

        WHEN ("a radial distance function is mapped to the grid"){

            std::function<TestType(Coordinate &c)> radius = [](Coordinate &c) { 
                return sqrt(pow(c.x,2)+pow(c.y,2)+pow(c.z,2));
            };

            grid.map(radius);

            THEN("any coordinate should return the radial distance from the origin"){
                Coordinate coord{-1,1,-1};
                TestType distance = sqrt(3);
                REQUIRE(grid.get(coord) == distance);
            }

        }

    }

}

TEMPLATE_TEST_CASE("Trilinear interpolation", "[interpolation]", 
        double, std::complex<double>){

    auto linspace = LinSpace{0,1,2};
    auto grid_space = GridSpace{linspace, linspace, linspace};

    GIVEN("a minimal 2x2x2 trilinear grid"){

        auto grid = TrilinearGrid<TestType>{grid_space};

        WHEN("the upper half of the grid (z=zmax) is set to 1"){
            grid.map([](Coordinate &c) -> TestType {return (c.z==1) ? 1.0 : 0.0;});

            THEN("exact points on z=min shold return 0.0"){
                REQUIRE(grid.get(Coordinate{0,0,0}) == 0.0);
            }

            AND_THEN("exact points on z=max shold return 1.0"){
                REQUIRE(grid.get(Coordinate{0,0,1}) == 1.0);
            }

            AND_THEN("a point in the z=zmax/2 plane should return 0.5"){
                REQUIRE(grid.get(Coordinate{0,0,0.5}) == 0.5);
            }

            AND_THEN("a point on an arbitrary z-fraction should return the correspoding fraction value"){
                REQUIRE(grid.get(Coordinate{0,0,0.1337}) == 0.1337);
                REQUIRE(grid.get(Coordinate{-4,-8,0.1337}) == 0.1337);
                REQUIRE(grid.get(Coordinate{10,5,0.1337}) == 0.1337);
            }
        }

    }

}

TEST_CASE("Performance test for grid mapping", "[.skip][map][benchmark]"){

    auto linspace = LinSpace{-1,1,801};
    auto grid_space = GridSpace{linspace, linspace, linspace};
    auto grid = NearestGrid<double>{grid_space};

    GIVEN ("an empty grid") {

        BENCHMARK ("a radial distance function is mapped to the grid"){

            std::function<double(Coordinate &c)> radius = [](Coordinate &c) { 
                return sqrt(pow(c.x,2)+pow(c.y,2)+pow(c.z,2));
            };

            grid.map(radius);

            THEN("any coordinate should return the radial distance from the origin"){
                Coordinate coord{-1,1,-1};
                double distance = sqrt(3);
                REQUIRE(grid.get(coord) == Approx(distance));
            }

        }
    }
}
