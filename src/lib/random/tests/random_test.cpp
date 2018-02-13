#include "gtest/gtest.h"
#include "random/random.hpp"
#include <math.h>
#include <limits.h>

constexpr int ITERATIONS = 10000000;
constexpr double EPSILON = 0.01;

using namespace sim;
using namespace random;

double get_mean_rate(Exponential &exp, const int iterations){
    double times = 0;
    for (int i = 0; i<iterations; i++){
        times += exp();
    }
    return 1/(times/iterations);
}

double get_mean(Normal &norm, const int iterations){
    double sample = 0;
    for (int i=0; i<iterations; i++){
        sample += norm(); 
    }
    return sample/iterations;
}

double get_sigma(Normal &norm, const int iterations){
    double samples = 0;
    double sample = 0;
    for (int i=0; i<iterations; i++){
        sample = norm(); 
        samples += pow(sample, 2);
    }
   return sqrt(samples/iterations);
}

double get_mean(Uniform &uni, const int iterations){
    double sample = 0;
    for (int i=0; i<iterations; i++){
        sample += uni(); 
    }
    return sample/iterations;
}

TEST (ExponentialTest, Zero){
    Exponential exp = Exponential(0.0, 0);
    ASSERT_FLOAT_EQ (DBL_MAX, exp());
}

TEST (ExponentialTest, Rate){
    Exponential exp = Exponential(3.1415, 0);
    double rate = get_mean_rate(exp, ITERATIONS);
    EXPECT_NEAR(rate, 3.1415, EPSILON);
}

TEST (ExponentialTest, ResetLambda){
    Exponential exp = Exponential(1.0, 0);
    double rate = get_mean_rate(exp, ITERATIONS);
    EXPECT_NEAR(rate, 1.0, EPSILON);
    exp.set_lambda(3.1415); 
    rate = get_mean_rate(exp, ITERATIONS);
    EXPECT_NEAR(rate, 3.1415, EPSILON);
}

TEST (NormalTest, OneMean){
    Normal norm = Normal(1.0);
    double mean = get_mean(norm, ITERATIONS); 
    EXPECT_NEAR(mean, 0.0, EPSILON); 
}

TEST (NormalTest, OneSigma){
    Normal norm = Normal(3.1415);
    double sigma = get_sigma(norm, ITERATIONS); 
    EXPECT_NEAR(sigma, 3.1415, EPSILON); 
}

TEST (UniformTest, Mean){
    Uniform uni = Uniform();
    double mean = get_mean(uni, ITERATIONS); 
    EXPECT_NEAR(mean, 0.5, EPSILON); 
}

TEST (UniformIntTest, Range){
    UniformInt uni = UniformInt(-400, 600);
    for (int i=0; i<ITERATIONS; i++){
        int result = uni();
        ASSERT_LE(result, 600);
        ASSERT_GE(result, -400); 
    } 
}
