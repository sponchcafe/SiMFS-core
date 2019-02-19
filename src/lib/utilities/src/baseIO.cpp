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

        void BaseIO::write_binary(void *data, size_t count){
            char *buffer = reinterpret_cast<char*> (data);
            ios.write(buffer, count);
        }

        void BaseIO::read_binary(void *data, size_t count){
            char *buffer = reinterpret_cast<char*> (data);
            ios.read(buffer, count);
        }

    }
}
