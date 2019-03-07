#include "catch.hpp"
#include "shape/XYZGauss.hpp"
#include "utils.hpp"
#include <cmath>

using namespace sim::focus;

/*--------------------------------------------------------------------------*/
TEST_CASE("Waist parameters of the gaussian focus function can be set", "[gauss][parameters]"){

    GIVEN("A gaussian focus function"){

        XYZGauss gauss{};

        THEN("Defaults are returned by get_json as json"){

            json params = gauss.get_json();
            REQUIRE(params.at("waist_x") == 249e-9);
            REQUIRE(params.at("waist_y") == 249e-9);
            REQUIRE(params.at("waist_z") == 637e-9);

        }

        WHEN("The waist parameters are set"){

            gauss.set_waists(100e-9, 200e-9, 300e-9);

            THEN("Changes are reflected in the parameters returned from get_json"){

                json params = gauss.get_json();
                REQUIRE(params.at("waist_x") == 100e-9);
                REQUIRE(params.at("waist_y") == 200e-9);
                REQUIRE(params.at("waist_z") == 300e-9);

            }

            WHEN("Waist parameters are overriden partially by set_json"){

                gauss.set_json(json{{"waist_x", 150e-9}});

                THEN("Changes are reflected in the parameters returned from get_json"){

                    json params = gauss.get_json();
                    REQUIRE(params.at("waist_x") == 150e-9);
                    REQUIRE(params.at("waist_y") == 200e-9);
                    REQUIRE(params.at("waist_z") == 300e-9);

                }

            }

        }

    }

}


/*--------------------------------------------------------------------------*/
TEST_CASE("The gaussian focus function is normalized", "[gauss][normalization]"){

    GIVEN("A gaussian focus function with some waist parameters"){

        XYZGauss gauss{};
        gauss.set_waists(100e-9, 200e-9, 300e-9);

        WHEN("The efficiency prefactor is used"){

            double pre = gauss.get_efficiency_prefactor();

            THEN("The result at (0,0,0) is 1.0"){

                double actual = pre * gauss.evaluate(0,0,0);
                REQUIRE(actual == 1.0);

            }

            double target = pre * std::pow(std::exp(1), -2);

            AND_THEN("The result at waist is 1/e^2 in x"){

                double actual = gauss.evaluate(100e-9, 0, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at waist is 1/e^2 in y"){

                double actual = gauss.evaluate(0, 200e-9, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at waist is 1/e^2 in z"){

                double actual = gauss.evaluate(0, 0, 300e-9);
                REQUIRE(actual == target);

            }

        }

        WHEN("The flux density prefactor is used"){

            double pre = gauss.get_flux_density_prefactor();

            THEN("The integral over the xy-plane at z=0 xy-plane is close to 1.0"){

                double xy_area = pre * integrate_xy(gauss);

                REQUIRE(xy_area == Approx(1.0));

            }

        }

    }

}
