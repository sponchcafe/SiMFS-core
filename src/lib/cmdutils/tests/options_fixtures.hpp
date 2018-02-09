#include "gtest/gtest.h"
#include "cmdutils/options.hpp"
    
using namespace sim::opt;

//-Convert-std::vector<std::string>-to-cstyle-char-**------------------------//
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

//-Convert-string-to-non-const-cstring---------------------------------------//
char * make_cstring(std::string s){
    char * cstring = new char[128];
    strcpy(cstring, s.c_str());
    return cstring;
}



//---------------------------------------------------------------------------//
class ParameterHandlerTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        ParameterHandlerTest() {}
        
        //-------------------------------------------------------------------//
        virtual void SetUp() {

            argv_mock = {"./execname", "./params.json", "anotherfile.txt", "1.9",
                "--dlong", "commandline", "-z", "something", "else"};

            // Create JSON parameter file
            
            json json_mock = json{
                {"test", 
                    {
                        {"clong", "json"}, 
                        {"dlong", "json"}, 
                        {"nested", "nested.json"}
                    }
                }
            };

            json nested_json_mock = json{
                {"nested_1", 22},
                {"nested_2", 23}
            };

            std::ofstream outfile("./params.json");
            outfile << json_mock;
            outfile.close();

            std::ofstream outfile2("./nested.json");
            outfile2 << nested_json_mock;
            outfile2.close();

        }

        //-Create-parameter-handler-from-argvmock----------------------------//
        void create_handler(std::vector<std::string> argv, 
                std::string prefix="test"){

            ParameterHandler *p = new ParameterHandler(
                    prefix,
                    argv.size(),
                    make_mock_argv(argv)
                    );

            std::string default_str = "default";
            p->get_parameter("along", 'a', default_str, "a description."); // 1
            p->get_parameter("blong", 'b', default_str, "b description."); // 2
            p->get_parameter("clong", 'c', default_str, "c description."); // 3
            p->get_parameter("dlong", 'd', default_str, "d description."); // 4

            handler.reset(p);

        }

        //-------------------------------------------------------------------//
        virtual void TearDown() {
            std::remove("./params.json");
            std::remove("./nested.json");
        }

        std::unique_ptr<ParameterHandler> handler;

        std::vector<std::string> argv_mock;

};
