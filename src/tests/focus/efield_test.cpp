#include "gtest/gtest.h"
#include "focus/efield.hpp"
#include <fstream>

using namespace sim::focus;
using namespace sim;

TEST(EFieldTest, Output){
    std::ofstream ofs{};
    ofs.open("field.dat");
    EField ef{};
    ef.init();

    EFieldGridSpec spec {
        {-2e-6, 2e-6, 32},
        {-2e-6, 2e-6, 32},
        {-2e-6, 2e-6, 32}
    };

    ofs.write(reinterpret_cast<char *>(&spec), sizeof(spec));

    EFieldComponents field{};
    double z_step = (spec.z.max-spec.z.min)/spec.z.n;
    double y_step = (spec.y.max-spec.y.min)/spec.y.n;
    double x_step = (spec.x.max-spec.x.min)/spec.x.n;
    for(double z=spec.z.min; z<spec.z.max; z+=z_step){
        for(double y=spec.y.min; y<spec.y.max; y+=y_step){
            for(double x=spec.x.min; x<spec.x.max; x+=x_step){
                field = ef.integrate_components(x, y, z);
                ofs.write(reinterpret_cast<char *>(&field), sizeof(field));
            } 
        }
    }
    ofs.close();

}

