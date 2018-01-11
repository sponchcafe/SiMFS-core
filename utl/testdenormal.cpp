#include <iostream>
#include <cmath>
using std::fpclassify;

void check_subnormal(double d){
    std::clog 
        << std::dec
        << d 
        << " is subnormal: " 
        << (std::fpclassify(d) == FP_SUBNORMAL)
        << " hexcode: "
        << std::hexfloat
        << d
        << std::endl;
}

int main(){

    check_subnormal(1.0);
    check_subnormal(1.0e-9);
    check_subnormal(1.0e-25);
    check_subnormal(1.0e-50);
    check_subnormal(1.0e-100);
    check_subnormal(1.0e-200);
    check_subnormal(1.0e-300);
    check_subnormal(1.0e-310);
    check_subnormal(1.0e-320);
    check_subnormal(5.0e-324);

}