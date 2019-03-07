#include "catch.hpp"
#include "shape/gaussBeam.hpp"
#include "utils.hpp"
#include <cmath>


using namespace sim::focus;

/*--------------------------------------------------------------------------*/
TEST_CASE("Parameter setting", "[gbeam][parameters]"){

    GIVEN("A gbeam focus function"){

        GaussBeam gbeam{};

        THEN("Defaults are returned by get_json as json"){

            json params = gbeam.get_json();
            REQUIRE(params.at("waist_xy") == 249e-9);
            REQUIRE(params.at("wavelength") == 488e-9);

        }

        WHEN("The waist parameters are set"){

            gbeam.set_waist(300e-9);
            gbeam.set_lambda(549e-9);

            THEN("Changes are reflected in the parameters returned from get_json"){

                json params = gbeam.get_json();
                REQUIRE(params.at("waist_xy") == 300e-9);
                REQUIRE(params.at("wavelength") == 549e-9);

            }

            WHEN("Parameters are overriden partially by set_json"){

                gbeam.set_json(json{{"waist_xy", 150e-9}});

                THEN("Changes are reflected in the parameters returned from get_json"){

                    json params = gbeam.get_json();
                    REQUIRE(params.at("waist_xy") == 150e-9);
                    REQUIRE(params.at("wavelength") == 549e-9);

                }

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("The gbeam focus function is normalized", "[gbeam][normalization]"){

    GIVEN("A gbeam focus function with some parameters"){

        GaussBeam gbeam{};
        gbeam.set_waist(100e-9);
        gbeam.set_lambda(488e-9);

        WHEN("The efficiency prefactor is used"){

            double pre = gbeam.get_efficiency_prefactor();

            THEN("The result at (0,0,0) is 1.0"){

                double actual = pre * gbeam.evaluate(0,0,0);
                REQUIRE(actual == 1.0);

            }

            double target = pre * std::pow(std::exp(1), -2);

            AND_THEN("The result at waist is 1/e^2 in x"){

                double actual = gbeam.evaluate(100e-9, 0, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at waist is 1/e^2 in y"){

                double actual = gbeam.evaluate(0, 100e-9, 0);
                REQUIRE(actual == target);

            }

            AND_THEN("The result at z=zR is 0.5"){

                double z_r = sim::CONST_PI * std::pow(100e-9, 2) / 488e-9;
                double actual = gbeam.evaluate(0.0, 0.0, z_r);
                REQUIRE(actual == Approx(0.5));

            }

        }

        WHEN("The flux density prefactor is used"){

            double pre = gbeam.get_flux_density_prefactor();
            double xy_area = 0.0;

            THEN("The integral over the xy-plane at z=0 xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(gbeam);
                REQUIRE(xy_area == Approx(1.0));

            }

            THEN("The integral over any sufficiently large xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(gbeam, 25e-9);
                REQUIRE(xy_area == Approx(1.0));

                xy_area = pre * integrate_xy(gbeam, 50e-9);
                REQUIRE(xy_area == Approx(1.0));

                xy_area = pre * integrate_xy(gbeam, -50e-9);
                REQUIRE(xy_area == Approx(1.0));

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("The gbeam focus function is normalized, even far away from z=0", "[gbeam][normalization][.slow]"){

    GIVEN("An gbeam focus function with some waist parameters"){

        GaussBeam gbeam{};
        gbeam.set_waist(100e-9);

        WHEN("The efficiency prefactor is used"){

            double pre = gbeam.get_flux_density_prefactor();
            double xy_area = 0.0;

            THEN("The integral over any sufficiently large xy-plane is close to 1.0"){

                xy_area = pre * integrate_xy(gbeam, 500e-9, -10e-6, 10e-6);
                REQUIRE(xy_area == Approx(1.0));

            }

        }

    }

}
