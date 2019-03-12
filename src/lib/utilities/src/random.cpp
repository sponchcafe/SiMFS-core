#include "random/random.hpp"
#include <iostream>
#include <cmath>
#include <limits>
#include <chrono>
#include "platform.hpp"

#ifdef WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif


using namespace std::chrono;

namespace sim{
    namespace random{

        static unsigned count = 0;

        unsigned get_new_seed(){
            milliseconds ms = duration_cast< milliseconds >(
                        system_clock::now().time_since_epoch()
                    );

            #ifdef WINDOWS
            auto pid = GetCurrentProcessId();
            #else
            auto pid = ::getpid();
            #endif

            return ms.count()*pid+(count++);
        }

        Exponential::Exponential(double lambda, unsigned seed){
            generator = std::mt19937(seed);
            set_lambda(lambda);
        }

        void Exponential::set_lambda(double lambda){
            Exponential::lambda = lambda;    
            distribution = std::exponential_distribution<double>(lambda);
        }

        double Exponential::get_lambda() const {
            return lambda;
        }

        double Exponential::operator()() {
            return distribution(generator);
        }



        Normal::Normal(double sigma, unsigned seed){
            generator = std::mt19937(seed);
            set_sigma(sigma);
        }

        void Normal::set_sigma(double sigma){
            Normal::sigma = sigma;
            distribution = std::normal_distribution<double>(0.0, sigma); // mu, sigma
        }

        double Normal::operator()() {
            return distribution(generator);  
        }



        Uniform::Uniform(unsigned seed){
            generator = std::mt19937(seed);
            distribution = std::uniform_real_distribution<double>(0.0, 1.0);
        }

        double Uniform::operator()(){
            return distribution(generator);
        }



        UniformInt::UniformInt(int min, int max, unsigned seed){
            generator = std::mt19937(seed);
            set_range(min, max);
        }

        void UniformInt::set_range(int min, int max){
            UniformInt::min = min;
            UniformInt::max = max;
            distribution = std::uniform_int_distribution<int>(min, max);
        }

        int UniformInt::operator()(){
            return distribution(generator);
        }
    }
}
