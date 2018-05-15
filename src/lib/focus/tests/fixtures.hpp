#include "gtest/gtest.h"
#include "focus/base.hpp"

using namespace sim::focus;
using namespace sim;

//-Testing-fixture-class-that-holds-a-focus-function-object------------------//
class EvaluationTest : public ::testing::Test{

    protected:

        EvaluationTest(){}
        virtual void SetUp(){}
        virtual void TearDown(){}

        double integrate_xy_plane(
                double z,
                double x_min,
                double x_max,
                double y_min,
                double y_max,
                double step)
        {
            double total = 0.0;
            for (double x=x_min; x<x_max; x+=step){
                for (double  y=y_min; y<y_max; y+=step){
                    //-Integration-over-dx-dy-step-step----------------------//
                    total += focus->evaluate(x, y, z) * step * step;
                }
            }
            return total;
        }

        Focus *focus;

};
