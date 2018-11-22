#pragma once 

#include <cmath>
#include <iostream>
#include "json/json.hpp"
#include "definitions/constants.hpp"
#include "definitions/types.hpp"

using json = nlohmann::json;

namespace sim{
    namespace focus{

        //-------------------------------------------------------------------//
        class Focus{

            public:  

                //-----------------------------------------------------------//
                virtual ~Focus(){};

                //-----------------------------------------------------------//
                virtual double evaluate(double x, double y, double z) const = 0;

                //-----------------------------------------------------------//
                virtual void set_json(json j) = 0;
                virtual json get_json() = 0;

                //-----------------------------------------------------------//
                virtual double get_flux_density_prefactor() const = 0;
                virtual double get_efficiency_prefactor() const = 0;
                void set_prefactor(double p) {
                    std::cerr << "set prefactor to " << p << '\n';
                    prefactor = p;}

            protected:


                double prefactor = 1.0;

        };

    }
}
