#include "jcli_fixtures.hpp"
#include <cstdlib>

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Default){
    std::string x = spec->get_option<std::string>(
            "xlong", {"x", "X"}, "default"
            );
    ASSERT_EQ(x, "default");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Environment){
    std::string d = spec->get_option<std::string>(
            "dlong", {"d", "D"}, "default"
            );
    ASSERT_EQ(d, "envd");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, UnnamedArgs){
    std::string c = spec->get_option<std::string>(
            "clong", {"c", "C"}, "default"
            );
    ASSERT_EQ(c, "argc");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, ShellOverwrite){
    std::string a = spec->get_option<std::string>(
            "along", {"a", "A"}, "default"
            );
    ASSERT_EQ(a, "shella");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, IncludeFile){
    json i = spec->get_option<json>(
            "include", {"i", "I"}, json{}
            );
    ASSERT_EQ(i["somekey"], "someval");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Multiples){
    int m = spec->get_option<int>(
            "multi", {"m", "M"}, -1 
            );
    ASSERT_EQ(m, 3);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, InvalidIgnore){
    int i = spec->get_option<int>(
            "invalid",  {}, -1
            );
    ASSERT_EQ(i, 2);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, InvalidReadThrough){
    int i = spec->get_option<int>(
            "invalid2",  {}, -1
            );
    ASSERT_EQ(i, 3);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, JsonParse){
    json p = spec->get_option<json>(
            "parse",  {}, json{}
            );
    ASSERT_EQ(p["a"], -6);
}
