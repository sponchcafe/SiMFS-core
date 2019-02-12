#pragma once

#include "shape/base.hpp"


/*--------------------------------------------------------------------------*/
/* Helper to integrate a focus function over an xy-plane                    
 * Note the HACK: Inlined function to prevent multiple definition error when 
 * including this header into more than one file. 
 *--------------------------------------------------------------------------*/
inline double integrate_xy(sim::focus::FocusShape &f, 
        double z=0.0, double min=-1e-6, double max=1e-6, double dxy=1e-9){

    double integral = 0.0;
    for (double x=min; x<=max; x+=dxy){
        for (double y=min; y<=max; y+=dxy){
            integral += f.evaluate(x, y, z) * dxy * dxy;
        }
    }
    return integral;

}
