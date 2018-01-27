#ifndef SIM_RANDOM_H
#define SIM_RANDOM_H 

#include <random>

namespace sim {
    namespace random {

        class Exponential{

            public:
                Exponential(double lambda, unsigned seed = 0);
                void set_lambda(double lambda);
                double get_lambda() const;
                double operator()();

            private:
                double lambda;
                std::mt19937 generator;
                std::exponential_distribution<double> distribution;  

        };

        class Normal{

            public:
                Normal(double sigma, unsigned seed = 0);
                void set_sigma(double s);
                double operator()();

            private:
                double sigma;
                std::mt19937 generator;
                std::normal_distribution<double> distribution;

        };

        class Uniform{

            public:
                Uniform(unsigned seed = 0); 
                double operator()();

            private:
                std::mt19937 generator;
                std::uniform_real_distribution<double> distribution;

        };

        class UniformInt{

            public:
                UniformInt(int min, int max, unsigned seed = 0);
                void set_range(int min, int max);
                int operator()();

            private:
                int min;
                int max;
                std::mt19937 generator;
                std::uniform_int_distribution<int> distribution;
        };
    }
}


#endif

