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
    FieldComponents field{};
    for(double z=-2e-6; z<2e-6; z+=1e-7){
        for(double y=-2e-6; y<2e-6; y+=1e-7){
            for(double x=-2e-6; x<2e-6; x+=1e-7){
                field = ef.integrate_components(x, y, z);
                ofs.write(reinterpret_cast<char *>(&field), sizeof(field));
            } 
        }
    }
    ofs.close();

}

