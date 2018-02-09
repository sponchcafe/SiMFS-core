#include "options_fixtures.hpp"
#include <cstdlib>


//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, NoConfig){
    create_handler(argv_mock);
    handler->enable_config();
    ASSERT_EQ(handler->in_config_mode(), false);
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage0_NoParam){
    create_handler(argv_mock);
    ASSERT_EQ(handler->params["nothere"], nullptr);
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage1_Default){
    create_handler(argv_mock);
    ASSERT_EQ(handler->params["along"], "default");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage2_JSON){
    create_handler(argv_mock);
    ASSERT_EQ(handler->params["clong"], "json");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Stage3_commandline){
    create_handler(argv_mock);
    ASSERT_EQ(handler->params["dlong"], "commandline");
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, ConfigJSON){

    argv_mock.push_back("-C");
    argv_mock.push_back("params");
    create_handler(argv_mock);

    //testing::internal::CaptureStdout();
    handler->enable_config();
    //std::string output = testing::internal::GetCapturedStdout();

    //json parsed = json::parse(output);
    //parsed = parsed["test"]; // prefix

    //ASSERT_EQ(handler->in_config_mode(), true);
    //ASSERT_EQ(parsed["along"], "default");
    //ASSERT_EQ(parsed["blong"], "environment");
    //ASSERT_EQ(parsed["clong"], "json");
    //ASSERT_EQ(parsed["dlong"], "commandline");

}



/*
//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, ConfigLog){

    argv_mock.push_back("-C");
    argv_mock.push_back("info");
    create_handler(argv_mock);

    testing::internal::CaptureStdout();
    handler->enable_config();
    std::string output = testing::internal::GetCapturedStdout();

    json parsed = json::parse(output);
    parsed = parsed["test"]; // prefix
    ASSERT_EQ(parsed["along"]["longoption"], "--along");
    ASSERT_EQ(parsed["blong"]["default"], "default");
    ASSERT_EQ(parsed["dlong"]["origin"], "commandline");
        
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, Help){

    argv_mock.push_back("-h");
    create_handler(argv_mock);

    testing::internal::CaptureStdout();
    handler->enable_config();
    handler->enable_help("Helpmessage");
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "Helpmessage\n");
        
}

//---------------------------------------------------------------------------//
TEST_F(ParameterHandlerTest, ParameterFile){
    argv_mock.push_back("-f");
    argv_mock.push_back("params.json");
    argv_mock.push_back("-C");
    argv_mock.push_back("params");
    create_handler(argv_mock);
    handler->enable_config();
    
}*/
