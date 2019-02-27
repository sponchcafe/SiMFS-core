#include "catch.hpp"
#include "imager/component.hpp"
#include "io/buffer.hpp"
#include <string>


using namespace sim;
using namespace sim::comp;
using namespace sim::io;


/*---------------------------------------------------------------------------*/
TEST_CASE("Imaging test", "[img][sanity]"){

    GIVEN("A vector of photons and a vector of coordinates"){

        std::vector<realtime_t> tags{0.1, 0.2, 0.3, 0.4, 0.5, 1.1, 1.2};
        std::vector<Coordinate> coords{
        Coordinate{0.0,0.0,0.0,0.0},
        Coordinate{1e-7,0.0,0.0,1.0}
        };

        WHEN("It is passed as single input to a mixer instance"){

            std::vector<std::string> tag_ids{"tags_1", "tags_2"};
            std::vector<std::string> coord_ids{"coords_1", "coords_2"};

            Imager img{};
            img.set_time_input_ids(tag_ids);
            img.set_coordinate_input_ids(coord_ids);

            std::vector<std::thread> threads{};

            for(auto id: tag_ids){
                threads.emplace_back(vector2buffer_thread<realtime_t>(tags, id));
            }
            for(auto id: coord_ids){
                threads.emplace_back(vector2buffer_thread<Coordinate>(coords, id));
            }

            threads.emplace_back(run_component<Imager>(img, true));

            for(auto &t: threads) t.join();

            THEN(""){
            }

        }

    }

}

