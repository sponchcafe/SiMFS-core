#include "catch.hpp"
#include "diffusion/dif_capi.h"
#include <json/json.hpp>

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

        destroy_diffusion(d);
    }

}

