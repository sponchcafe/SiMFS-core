#include <sim/logging.hpp>

namespace sim{

    namespace log{
    
        void info(std::string msg) {
            std::clog << msg;
        }
        void warn(std::string msg) {
            std::cerr << msg;
        }
        void critical(std::string msg) {
            std::cerr << msg; exit(EXIT_FAILURE);
        }
    };

};
