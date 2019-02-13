#include "catch.hpp"
#include "shape/main.hpp"
#include <string>
#include "json/json.hpp"

using namespace sim::focus;
using json = nlohmann::json;

/*--------------------------------------------------------------------------*/
TEST_CASE("The focus factory generates the right focus shape objects", "[factory]"){

    GIVEN("A focus shape type"){

        ShapeType type = ShapeType::XYZ_GAUSS;

        WHEN("The passed to the focus shape factory"){

            auto fs = focus_shape_factory(type);

            THEN("The returned object is a unique_ptr to the appropriate focus shape object"){

                // try to cast the returned ptr to the specific subtype
                REQUIRE(dynamic_cast<XYZGauss *>(fs.get()));

            }

            AND_THEN("not another focus type"){

                // try to cast the returned ptr to the specific subtype
                REQUIRE_FALSE(dynamic_cast<XYGaussZExp *>(fs.get()));
                REQUIRE_FALSE(dynamic_cast<GaussBeam *>(fs.get()));
                REQUIRE_FALSE(dynamic_cast<EField *>(fs.get()));

            }

        }

        AND_GIVEN("A parameter json"){

            json params{
                {"waist_x", 100e-9},
                {"waist_y", 200e-9},
                {"waist_z", 300e-9}
            };

            WHEN("passed to the focus shape factory"){

                auto fs = focus_shape_factory(type, params);

                THEN("the created focus shape is configured with the paramters"){

                    json actual = fs->get_json();
                    REQUIRE(actual["waist_x"].get<double>() == 100e-9);
                    REQUIRE(actual["waist_y"].get<double>() == 200e-9);
                    REQUIRE(actual["waist_z"].get<double>() == 300e-9);

                }

            }

        }

    }

}
