#include "catch.hpp"
#include "baseIO/baseIO.hpp"
#include <sstream>
#include <vector>

using namespace sim;
using namespace sim::io;

/*--------------------------------------------------------------------------*/
TEST_CASE("BaseIO can be used to serialize binary data", "[IO]"){

    GIVEN ("A BaseIO instance"){

        auto ss = std::stringstream();
        auto io = BaseIO(ss);

        std::vector<double> out_data{1,2,3,4,5};
        std::vector<double> in_data(out_data.size());

        char *out_buffer = reinterpret_cast<char *>(out_data.data());
        char *in_buffer = reinterpret_cast<char *>(in_data.data());

        WHEN ("Data is written to the stream"){

            io.write(out_buffer, out_data.size()*sizeof(double));

            THEN("Data is available in the output stream"){

                io.read(in_buffer, in_data.size()*sizeof(double));

                REQUIRE(in_data == out_data);
                
            }

        }

    }

}

