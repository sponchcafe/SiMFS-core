#include "gtest/gtest.h"
#include "focus/alpha.hpp"
#include "focus/base.hpp"
#include "fixtures.hpp"

using namespace sim::focus;
using namespace sim;

constexpr double WAIST = 100e-9;
constexpr double MAX_VAL = 63661977236758.16; 
constexpr double EPSILON = 1e-12;

class AlphaGaussTest : public EvaluationTest{
    protected:
        void SetUp(){
            focus = new Alpha(WAIST, WAIST);
        }
};


TEST_F(AlphaGaussTest, Zero){
    /* Test central amplitude == 1 */
    EXPECT_DOUBLE_EQ(focus->evaluate(0.0, 0.0, 0.0), MAX_VAL);
}

TEST_F(AlphaGaussTest, InflectionX){
    /* Test inflection point at waist == 1/e^2 */
    EXPECT_DOUBLE_EQ(focus->evaluate(WAIST, 0.0, 0.0), MAX_VAL/pow(CONST_E, 2));
}

TEST_F(AlphaGaussTest, InflectionZ){
    /* Test inflection point at waist == 1/e^2 */
    EXPECT_DOUBLE_EQ(focus->evaluate(0.0, 0.0, WAIST), MAX_VAL/pow(CONST_E, 2));
}

TEST_F(AlphaGaussTest, Point){
    /* Test arbitrary precalculated point */
    EXPECT_DOUBLE_EQ(focus->evaluate(314e-9, 157e-9, 99e-9), 278611371419.2002); 
}

TEST_F(AlphaGaussTest, AreaXYZero){
    /* Test normalized area in xy z=0, integrating over 5 waist range with 1nm stepsize */
    double area = integrate_xy_plane(0, -1e-6, 1e-6, -1e-6, 1e-6, 1e-9);
    EXPECT_NEAR(area, 1.0, EPSILON);
}

TEST_F(AlphaGaussTest, AreaXY_HalfWaist){
    /* Test normalized area in xy z=1, integrating over 5 waist range with 1nm stepsize */
    double area = integrate_xy_plane(0.5*WAIST, -10*WAIST, 10*WAIST, -10*WAIST, 10*WAIST, 1e-9);
    EXPECT_NEAR(area, 1.0, EPSILON);
}

TEST_F(AlphaGaussTest, AreaXY_DoubleWaist){
    /* Test normalized area in xy z=1, integrating over 5 waist range with 1nm stepsize */
    double area = integrate_xy_plane(2*WAIST, -10000*WAIST, 10000*WAIST, -10000*WAIST, 10000*WAIST, 1000e-9);
    EXPECT_NEAR(area, 1.0, EPSILON);
}
