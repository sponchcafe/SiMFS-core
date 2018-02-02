#ifndef SIM_LOG_H
#define SIM_LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

namespace sim{

    namespace log{
    
        void info(std::string msg);
        void warn(std::string msg);
        void critical(std::string msg);

    };

}

#endif
