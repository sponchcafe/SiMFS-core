#include "gtest/gtest.h"
#include "focus/gaussian.hpp"
#include "fixtures.hpp"

using namespace sim::focus;
using namespace sim;

constexpr double WAIST = 100e-9;

class GaussTest : public EvaluationTest{
    protected:
        void SetUp(){
            focus = new Gaussian(WAIST, WAIST, WAIST);
        }
};

TEST_F(GaussTest, Zero){
    /* Test central amplitude == 1 */
    EXPECT_DOUBLE_EQ(focus->evaluate(0.0, 0.0, 0.0), 1.0);
}

TEST_F(GaussTest, Inflection){
    /* Test inflection point at waist == 1/e^2 */
    EXPECT_DOUBLE_EQ(focus->evaluate(WAIST, 0.0, 0.0), 1/pow(CONST_E, 2));
}

TEST_F(GaussTest, Point){
    /* Test arbitrary precalculated point */
    EXPECT_DOUBLE_EQ(focus->evaluate(314e-9, 157e-9, 99e-9), 2.7782496745139421e-12);
}
