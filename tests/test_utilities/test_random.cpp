#include "catch.hpp"
#include "random/random.hpp"

using namespace sim::random;


/*--------------------------------------------------------------------------*/
TEST_CASE("Get seed", "[random][seed]"){

    SECTION("Seeds are non-zero and unique"){

        REQUIRE(get_new_seed() != 0);
        unsigned s1 = get_new_seed();
        unsigned s2 = get_new_seed();
        REQUIRE(s1 != s2);

    }

}


/*--------------------------------------------------------------------------*/
TEST_CASE("Exponential distribution", "[random][exponential]"){

    GIVEN("An exponential distribution"){

        Exponential exp{1e+3, get_new_seed()};

        THEN("get_lambda returns the lambda parameter"){

            REQUIRE(exp.get_lambda() == 1e+3);

        }

        THEN("Calling the distribution yields a random value > 0"){

            double x1 = exp();
            double x2 = exp();

            REQUIRE(x1 > 0);
            REQUIRE(x2 > 0);
            REQUIRE(x1 != x2);

        }

        THEN("The average rate is close to 1/lambda"){

            double sum = 0.0;
            int N = 1<<22;
            for(int i=0; i<N; i++) sum += exp();
            double average = sum/N;
            REQUIRE(average == Approx(0.001).epsilon(1e-2));

        }
        
    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Normal distribution", "[random][normal]"){

    GIVEN("A normal distribution"){

        Normal normal{3.14, get_new_seed()};

        THEN("Calling the distribution yields a random value"){
            
            REQUIRE(normal() != 0.0);

        }

        THEN("The average value is zero"){
            
            double sum = 0.0;
            int N = 1<<22;
            for(int i=0; i<N; i++) sum += normal();
            double average = sum/N;
            REQUIRE(average == Approx(0).margin(0.1));

        }
        
        THEN("The stddev of the distribution is sigma"){

            double sum = 0.0;
            int N = 1<<22;
            for(int i=0; i<N; i++) sum += std::pow(normal(), 2);
            double stddev = std::sqrt(sum/N);
            REQUIRE(stddev == Approx(3.14).margin(0.1));
        
        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Uniform distribution", "[random][uniform]"){
    
    GIVEN("A uniform distribution"){

        Uniform uniform{get_new_seed()};

        THEN("Calling the distribution yields a random value 0 < x <= 1.0"){

            double actual = uniform();
            REQUIRE(actual > 0.0);
            REQUIRE(actual <= 1.0);

        }

        THEN("The average value is close to 0.5"){

            double sum = 0.0;
            int N = 1<<22;
            for(int i=0; i<N; i++) sum += uniform();
            double average = sum/N;
            REQUIRE(average == Approx(0.5).margin(0.001));

        }

        THEN("The variance of the distribution is 1/12"){

            double sum = 0.0;
            int N = 1<<22;
            for(int i=0; i<N; i++) sum += std::pow(uniform()-0.5, 2);
            double variance = sum/N;
            REQUIRE(variance == Approx(1.0/12).margin(0.1));
        
        }
    
    }

}

