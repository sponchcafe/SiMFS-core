#include "gtest/gtest.h"
#include "focus/base.hpp"

using namespace sim::focus;
using namespace sim::io; 

class EvaluationTest : public ::testing::Test{
    /* Testing fixure class that initializes a focus funciton object.
     */
    protected:
        EvaluationTest(){}
        virtual void SetUp(){}
        virtual void TearDown(){}

        double integrate_xy_plane(si_coord_t z, si_coord_t x_min, si_coord_t x_max, si_coord_t y_min, si_coord_t y_max, si_coord_t step){
            double total = 0.0;
            c.z = z;
            for (si_coord_t x=x_min; x<x_max; x+=step){
                for (si_coord_t y=y_min; y<y_max; y+=step){
                    c.x = x;
                    c.y = y;
                    total += focus->evaluate(c) * step * step; // for delta xy
                }
            }
            return total;
        }

        BaseFocus *focus;
        SI_Coordinate c;

};
