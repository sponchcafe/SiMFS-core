#include "catch.hpp"
#include "shape/XYGaussZExp.hpp"
#include "utils.hpp"
#include <cmath>

using namespace sim::focus;

/*--------------------------------------------------------------------------*/
TEST_CASE("Waist parameters of the alpha focus function can be set", "[alpha][parameters]"){

    GIVEN("An alpha focus function"){

        XYGaussZExp alpha{};

        THEN("Defaults are returned by get_json as json"){

            json params = alpha.get_json();
            REQUIRE(params.at("waist_xy") == 249e-9);
            REQUIRE(params.at("waist_z") == 637e-9);

        }

        WHEN("The waist parameters are set"){

            alpha.set_waists(100e-9, 300e-9);

            THEN("Changes are reflected in the parameters returned from get_json"){

                json params = alpha.get_json();
                REQUIRE(params.at("waist_xy") == 100e-9);
                REQUIRE(params.at("waist_z") == 300e-9);

            }

            WHEN("Waist parameters are overriden partially by set_json"){

                alpha.set_json(json{{"waist_xy", 150e-9}});

                THEN("Changes are reflected in the parameters returned from get_json"){

                    json params = alpha.get_json();
                    REQUIRE(params.at("waist_xy") == 150e-9);
                    REQUIRE(params.at("waist_z") == 300e-9);

                }

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("The alpha focus function is normalized", "[alpha][normalization]"){

    GIVEN("An alpha focus function with some waist parameters"){

        XYGaussZExp alpha{};
        alpha.set_waists(100e-9, 300e-9);

        WHEN("The efficiency prefactor is used"){

            double pre = alpha.get_efficiency_prefactor();

            THEN("The result at (0,0,0) is 1.0"){

                double actual = pre * alpha.evaluate(0,0,0);
                REQUIRE(actual == 1.0);

            }

            double target = pre * std::pow(std::exp(1), -2);

            AND_THEN("The result at waist is 1/e^2 in x"){

                double actual = alpha.evaluate(100e-9, 0, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at waist is 1/e^2 in y"){

                double actual = alpha.evaluate(0, 100e-9, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at waist is 1/e^2 in z"){

                double actual = alpha.evaluate(0, 0, 300e-9);
                REQUIRE(actual == target);

            }

        }

        WHEN("The flux density prefactor is used"){

            double pre = alpha.get_flux_density_prefactor();
            double xy_area = 0.0;

            THEN("The integral over the xy-plane at z=0 xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(alpha);
                REQUIRE(xy_area == Approx(1.0));

            }

            THEN("The integral over any sufficiently large xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(alpha, 25e-9);
                REQUIRE(xy_area == Approx(1.0));

                xy_area = pre * integrate_xy(alpha, 150e-9);
                REQUIRE(xy_area == Approx(1.0));

                xy_area = pre * integrate_xy(alpha, -200e-9);
                REQUIRE(xy_area == Approx(1.0));

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("The alpha focus function is normalized, even far away from z=0", "[alpha][normalization][.slow]"){

    GIVEN("An alpha focus function with some waist parameters"){

        XYGaussZExp alpha{};
        alpha.set_waists(100e-9, 300e-9);

        WHEN("The efficiency prefactor is used"){

            double pre = alpha.get_flux_density_prefactor();
            double xy_area = 0.0;

            THEN("The integral over any sufficiently large xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(alpha, 500e-9, -10e-6, 10e-6);
                REQUIRE(xy_area == Approx(1.0));

            }

        }

    }

}

