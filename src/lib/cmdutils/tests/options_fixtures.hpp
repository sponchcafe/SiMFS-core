#include "gtest/gtest.h"
#include "cmdutils/options.hpp"
    
using namespace sim::opt;

//-Convert-std::vector<std::string>-to-cstyle-char-**-----------------------//
char ** make_mock_argv(std::vector<std::string> &string_vector){
    char ** cstrings = new char *[string_vector.size()+1];
    int pos = 0;
    for (auto it : string_vector){
        char * cstr = new char [it.length()+1];
        std::strcpy(cstr, it.c_str());
        cstrings[pos] = cstr;
        pos++;
    }
    cstrings[pos] = NULL;
    return cstrings;
}


//---------------------------------------------------------------------------//
class ParameterHandlerTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        ParameterHandlerTest() :
            argv_mock{"./execname", "-d", "commandline"},
            json_mock{ {"test", {{"clong", "json"}, {"dlong", "json"}}} }
        {}
        
        //-------------------------------------------------------------------//
        virtual void SetUp() {

            char environment[] = "blong=environment";
            putenv(environment);

            p = ParameterHandler(
                    "test", 
                    json_mock,
                    argv_mock.size(),
                    make_mock_argv(argv_mock));

            std::string default_str = "default";
            p.get_parameter("along", 'a', default_str, "a description."); // 1.
            p.get_parameter("blong", 'b', default_str, "b description."); // 2.
            p.get_parameter("clong", 'c', default_str, "c description."); // 3.
            p.get_parameter("dlong", 'd', default_str, "d description."); // 4.

        }

        //-------------------------------------------------------------------//
        virtual void TearDown() {
        }

        std::vector<std::string> argv_mock;
        json json_mock;
        ParameterHandler p;

};

//---------------------------------------------------------------------------//
class ParameterHandlerDebugTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        ParameterHandlerDebugTest() :
            argv_mock{"./execname", "-d", "commandline", "-C"},
            json_mock{ {"test", {{"clong", "json"}, {"dlong", "json"}}} }
        {}
        
        //-------------------------------------------------------------------//
        virtual void SetUp() {

            char environment[] = "blong=environment";
            putenv(environment);

            p = ParameterHandler(
                    "test", 
                    json_mock,
                    argv_mock.size(),
                    make_mock_argv(argv_mock));

            std::string default_str = "default";
            p.get_parameter("along", 'a', default_str, "a description."); // 1.
            p.get_parameter("blong", 'b', default_str, "b description."); // 2.
            p.get_parameter("clong", 'c', default_str, "c description."); // 3.
            p.get_parameter("dlong", 'd', default_str, "d description."); // 4.

        }

        //-------------------------------------------------------------------//
        virtual void TearDown() {
        }

        std::vector<std::string> argv_mock;
        json json_mock;
        ParameterHandler p;

};
