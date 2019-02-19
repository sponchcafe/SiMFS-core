#include "baseIO/baseIO.hpp"

namespace sim{
    namespace io{

        BaseIO::BaseIO(std::iostream &ios) : ios(ios) {}
        void BaseIO::write(const char *data, size_t count) {
            ios.write(data, count);
        }
        void BaseIO::read(char *data, size_t count) {
            ios.read(data, count);
        }

    }
}
