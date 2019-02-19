#pragma once
#include <iostream>

namespace sim{
    namespace io{

        class BaseIO{

            public:

                BaseIO(std::iostream &ios);
                void write(const char *data, size_t count);
                void read(char *data, size_t count);
                void write_binary(void *data, size_t count);
                void read_binary(void *data, size_t count);

            protected:

                std::iostream &ios;

        };

    }
}
