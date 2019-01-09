#include "catch.hpp"
#include "component/component.hpp"


using namespace sim::comp;
using json = nlohmann::json;

/*--------------------------------------------------------------------------*/
class ComponentMock : Component {

    public: 

        ComponentMock(int &rep) : reporter{rep} {}

        void set_json(json j){

            json params = get_json();
            params.merge_patch(j);

            i1 = params.at("i1");
            d1 = params.at("d1");
            s1 = params.at("s1");

        }

        json get_json(){

            json j{};
            j["i1"] = i1;
            j["d1"] = d1;
            j["s1"] = s1;

            return j;
        }

        void run() {
            i1 += 2;
            reporter = i1;
        }

        void init() {
            i1 += 1;
            reporter = i1;
        }

        int i1 = 0;
        double d1 = 0.0;
        std::string s1 = "";
        int &reporter;

};

/*--------------------------------------------------------------------------*/
TEST_CASE("An inherited components needs to implement the purely virtual functions of Component", "[component]"){

    int reporter;

    GIVEN("An instance of the derived component"){

        ComponentMock c{reporter};

        WHEN("Data is set via set_json"){

            json j{{"i1", 22}, {"d1", 3.14}, {"s1", "test"}};
            c.set_json(j);
            
            THEN("The changes are reflected in the result of get_json"){
                
                json result = c.get_json();
                REQUIRE(result["i1"] == 22);
                REQUIRE(result["d1"] == 3.14);
                REQUIRE(result["s1"] == "test");

            }

        }

        WHEN("Partial data is set via set_json"){

            json j{{"i1", 22}};
            c.set_json(j);
            
            THEN("The changes are reflected in the result of get_json while defaults are preserved"){
                
                json result = c.get_json();
                REQUIRE(result["i1"] == 22);
                REQUIRE(result["d1"] == 0.0);
                REQUIRE(result["s1"] == "");

            }

        }

        WHEN("No data is set via set_json"){

            THEN("The defaults are preserved in the result of get_json"){
                
                json result = c.get_json();
                REQUIRE(result["i1"] == 0);
                REQUIRE(result["d1"] == 0.0);
                REQUIRE(result["s1"] == "");

            }
        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Component::init and Component::run can be overloaded", "[component]"){

    int reporter;

    GIVEN("An instance of the derived component"){

        ComponentMock c{reporter};

        WHEN("It is initialized and run"){

            c.init();
            c.run();

            THEN("The state of the component represents the correct state after 1. init and 2. run"){

                REQUIRE(c.i1 == 3);

            }

        }

    }

}

/*--------------------------------------------------------------------------*/
TEST_CASE("Components can be run in a seperate thread", "[component][threaded]"){

    int reporter;

    GIVEN("An instance of the derived component"){

        ComponentMock c{reporter};

        WHEN("It is initialized and run in a thread without the init flag"){

            c.init();
            std::thread run_thr = run_component<ComponentMock>(c);

            THEN("The component runs"){

                run_thr.join();
                REQUIRE(reporter == 3);

            }

        }

        WHEN("It is run in a thread with the init flag"){

            std::thread run_thr = run_component<ComponentMock>(c, true);

            THEN("The component runs with automatic initialization"){

                run_thr.join();
                REQUIRE(reporter == 3);

            }

        }

    }
    
}
