#include <cstdlib> // putenv
#include "catch.hpp"
#include "env/env.hpp"
#include <string>
#include <iostream>


using namespace sim;


/*--------------------------------------------------------------------------*/
TEST_CASE("String environment variable", "[env][string]"){

    GIVEN ("An string environment variable"){

        putenv(const_cast<char *>("ENVVAR=some_value"));

        THEN("Getting the variable by name returns its value as a string"){

            std::string result = get_env("ENVVAR", "");
            REQUIRE(result == "some_value");

        }

        THEN("Getting an undfined variable returns the default string"){

            std::string result = get_env("UNDEFINED", "default");
            REQUIRE(result == "default");

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Integer environment variable", "[env][int]"){

    GIVEN ("An int environment variable"){

        putenv(const_cast<char *>("ENVVAR=22"));

        THEN("Getting the variable by name returns its value as an int"){

            int result = get_env("ENVVAR", -1);
            REQUIRE(result == 22);

        }

        THEN("Getting an undfined variable returns the default int"){

            int result = get_env("UNDEFINED", -1);
            REQUIRE(result == -1);

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Double environment variable", "[env][double]"){

    GIVEN("A double envrionment variable"){

        putenv(const_cast<char *>("ENVVAR=3.14"));

        THEN("Getting the variable by name returns its value as a double"){

            double result = get_env("ENVVAR", -1.0);
            REQUIRE(result == 3.14);

        }

        THEN("Getting an undfined variable returns the default double"){

            double result = get_env("UNDEFINED", -1.0);
            REQUIRE(result == -1);

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Byte environment variable", "[env][byte]"){

    GIVEN("A byte suffix envrionment variable (no suffix)"){

        putenv(const_cast<char *>("ENVVAR=1024"));

        THEN("Getting the variable by name returns the expanded byte count"){

            unsigned long int result = get_env_bytes("ENVVAR");
            REQUIRE(result == 1024);

        }

    }

    GIVEN("A byte suffix envrionment variable (k)"){

        putenv(const_cast<char *>("ENVVAR=1024k"));

        THEN("Getting the variable by name returns the expanded byte count"){

            unsigned long int result = get_env_bytes("ENVVAR");
            REQUIRE(result == 1024*1024);

        }

    }

    GIVEN("A byte suffix envrionment variable (M)"){

        putenv(const_cast<char *>("ENVVAR=1024M"));

        THEN("Getting the variable by name returns the expanded byte count"){

            unsigned long int result = get_env_bytes("ENVVAR");
            REQUIRE(result == 1024*1024*1024);

        }

    }

    GIVEN("A byte suffix envrionment variable (G)"){

        putenv(const_cast<char *>("ENVVAR=1024G"));

        THEN("Getting the variable by name returns the expanded byte count"){

            unsigned long int result = get_env_bytes("ENVVAR");
            REQUIRE(result == 1024*1024*1024*1024L);

        }

    }

} 

/*--------------------------------------------------------------------------*/
TEST_CASE("Time environment variable", "[env][time]"){

    GIVEN("A time suffix envrionment variable (nano)"){

        putenv(const_cast<char *>("ENVVAR=1n"));

        THEN("Getting the variable by name returns the expanded nanosecond count"){

            unsigned long int result = get_env_nanos("ENVVAR");
            REQUIRE(result == 1);

        }

    }

    GIVEN("A time suffix envrionment variable (micro)"){

        putenv(const_cast<char *>("ENVVAR=1u"));

        THEN("Getting the variable by name returns the expanded nanosecond count"){

            unsigned long int result = get_env_nanos("ENVVAR");
            REQUIRE(result == 1000);

        }

    }

    GIVEN("A time suffix envrionment variable (milli)"){

        putenv(const_cast<char *>("ENVVAR=1m"));

        THEN("Getting the variable by name returns the expanded nanosecond count"){

            unsigned long int result = get_env_nanos("ENVVAR");
            REQUIRE(result == 1000000);

        }

    }

    GIVEN("A time suffix envrionment variable (no suffix)"){

        putenv(const_cast<char *>("ENVVAR=1"));

        THEN("Getting the variable by name returns the expanded nanosecond count"){

            unsigned long int result = get_env_nanos("ENVVAR");
            REQUIRE(result == 1000000000);

        }

    }

}
