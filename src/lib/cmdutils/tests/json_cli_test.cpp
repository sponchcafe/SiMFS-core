#include "json_cli_fixtures.hpp"
#include "cmdutils/json_cli.hpp"

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, MockArgv){
    add_one_argv("someoption");
    char ** argv_mock = get_argv_mock();
    testing::internal::CaptureStdout();
    for (int i=0; i<argc_mock; i++) std::cout << argv_mock[i] << std::endl;
    std::string output = testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "./execname\nsomeoption\n");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Empty){
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args[GLOBALS_KEY], json::array());
    ASSERT_EQ(jsoncli.args[GLOBALS_KEY], json::array());
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Execname){
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args[EXECNAME_KEY], "./execname");
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Globals){
    add_many_argv({"g1", "g2"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args[GLOBALS_KEY].size(), 2);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Flag){
    add_many_argv({"-x"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["x"], true);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Shortopt){
    add_many_argv({"-x", "21"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["x"], 21);
}
    
//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Longopt){
    add_many_argv({"--xopt", "21"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["xopt"], 21);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Paramlist){
    add_many_argv({"--xopt", "21", "22", "23"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["xopt"].size(), 3);
    ASSERT_EQ(jsoncli.args["xopt"][0], 21);
    ASSERT_EQ(jsoncli.args["xopt"][1], 22);
    ASSERT_EQ(jsoncli.args["xopt"][2], 23);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, External){
    add_many_argv({"--external", "external.json"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["external"]["exval"], 22.4);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, Combined){
    add_many_argv({"--someopt", "external.json"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["someopt"]["exval"], 22.4);
}

//---------------------------------------------------------------------------//
TEST_F(JsonCliTest, PropagateGlobals){
    add_many_argv({"external.json", "external2.json", "Some other global"});
    JsonCLI jsoncli = get_json_cli_to_test();
    ASSERT_EQ(jsoncli.args["exval"], 42);
}

