#include "gtest/gtest.h"
#include "focus/gaussian.hpp"
#include "focus_test_fixture.hpp"

using namespace sim::focus;
using namespace sim::io; 

constexpr si_coord_t WAIST = 100e-9;

class GaussTest : public EvaluationTest{
    protected:
        void SetUp(){
            focus = new Gaussian(WAIST, WAIST, WAIST);
        }
};

TEST_F(GaussTest, Zero){
    /* Test central amplitude == 1 */
    EXPECT_DOUBLE_EQ(focus->evaluate(c), 1.0);
}

TEST_F(GaussTest, Inflection){
    /* Test inflection point at waist == 1/e^2 */
    c.x = WAIST;
    EXPECT_DOUBLE_EQ(focus->evaluate(c), 1/pow(CONST_E, 2));
}

TEST_F(GaussTest, Point){
    /* Test arbitrary precalculated point */
    c = SI_Coordinate{314e-9, 157e-9, 99e-9};
    EXPECT_DOUBLE_EQ(focus->evaluate(c), 2.7782496745139421e-12);
}
