#pragma once
#include <iostream>

namespace sim{
    namespace io{

        class BaseIO{

            public:

                BaseIO(std::iostream &ios);
                void write(const char *data, size_t count);
                void read(char *data, size_t count);

            private:

                std::iostream &ios;


        };

    }
}
