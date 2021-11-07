#include "catch.hpp"
#include "diffusion/dif_capi.h"
#include <json/json.hpp>
#include <cstdio>
#include <thread>

using json = nlohmann::json;


/*---------------------------------------------------------------------------*/
TEST_CASE("Create configure and destroy Diffusion via capi", "[capi]"){

    GIVEN("a new diffusion instance created via the c api") {
        Diffusion *d = make_diffusion();

        json cfg;
        char buf[1024];
        WHEN("the json config is read via the c api") {
            get_json(d, buf);

            THEN("the default values are reflected") {
                cfg = json::parse(buf);
                REQUIRE(cfg["radius"] == 1e-6);
            }
        }

        WHEN("the config is changed via the c api") {
            cfg["radius"] = 2e-6;
            auto new_cfg_str = cfg.dump();
            set_json(d, new_cfg_str.c_str());

            THEN("the changes are reflected") {
                get_json(d, buf);
                cfg = json::parse(buf);
                REQUIRE(cfg["radius"] == 2e-6);
            }
        }

    }

}



/*---------------------------------------------------------------------------*/
TEST_CASE("Read coordinates", "[capi]"){
    GIVEN("a new diffusion instance created via the c api") {
        Diffusion *d = make_diffusion();

        char buf[1024];
        get_json(d, buf);
        auto cfg = json::parse(buf);
        cfg["experiment_time"] = 1e-3;
        strcpy(buf, cfg.dump().c_str());
        set_json(d, buf);
        auto time = cfg["experiment_time"];

        WHEN("outputs are initialized and the component is run") {
            THEN("coordinate data can be read") {

                Coordinate c;
                double col;
                int n=0;

                init(d);

                auto t1 = std::thread{[&](){
                    init_coord_io(d);
                    while(next_coordinate(d, &c)) { n++; }
                }};
                auto t2 = std::thread{[&](){
                    init_collision_io(d);
                    while(next_collision(d, &col)) { }
                }};
                auto t3 = std::thread{[&](){run(d);}};
                t3.join();
                t2.join();
                t1.join();

                AND_THEN("all coordinates are read"){
                    REQUIRE(n == 10000);
                }

            }
        }

        destroy_diffusion(d);
    }
}
