#include "options_fixtures.hpp"
#include <cstdlib>

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, NoConfig){
    ASSERT_EQ(p.in_debug_mode(), false);
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage0_NoParam){
    ASSERT_EQ(p.params["nothere"], nullptr);
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage1_Default){
    ASSERT_EQ(p.params["along"], "default");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage2_Environment){
    ASSERT_EQ(p.params["blong"], "environment");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage3_JSON){
    ASSERT_EQ(p.params["clong"], "json");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage4_commandline){
    ASSERT_EQ(p.params["dlong"], "commandline");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerDebugTest, Config){
    testing::internal::CaptureStdout();
    p.enableConfig();
    std::string output = testing::internal::GetCapturedStdout();
    json parsed = json::parse(output);
    ASSERT_EQ(p.in_debug_mode(), true);
    ASSERT_EQ(parsed["along"], "default");
    ASSERT_EQ(parsed["blong"], "environment");
    ASSERT_EQ(parsed["clong"], "json");
    ASSERT_EQ(parsed["dlong"], "commandline");
}
