#include "gtest/gtest.h"
#include "cmdutils/json_cli.hpp"

using namespace sim::opt;


//---------------------------------------------------------------------------//
class JsonCliTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        JsonCliTest() {}

        //-------------------------------------------------------------------//
        virtual void SetUp() {
            add_one_argv("./execname");
            json external;
            external["exval"] = 22.4;
            std::ofstream outfile("./external.json");
            outfile << external.dump(4);
            outfile.close();
            json external2;
            external2["exval2"] = "someval";
            external2["exval"] = 42;
            std::ofstream outfile2("./external2.json");
            outfile2 << external2.dump(4);
            outfile2.close();

            std::string setenv = "SIM_PARAM_FILE=env_params.json";

            putenv(make_cstring(setenv));

        }

        //-------------------------------------------------------------------//
        virtual void TearDown() {}

        JsonCLI get_json_cli_to_test(){
            JsonCLI jsoncli = JsonCLI(argc_mock, get_argv_mock());
            return jsoncli;
        }

        void add_many_argv(std::vector<std::string> vec){
            for (auto it: vec) add_one_argv(it);
        }

        void add_one_argv(std::string s){
            argv_mock_vec.push_back(s);
            argc_mock = argv_mock_vec.size();
            argv_mock.reset(make_argv_mock(argv_mock_vec));
        }

        char ** get_argv_mock(){
            return argv_mock.get();
        }


        //-Convert-std::vector<std::string>-to-cstyle-char-**----------------//
        char ** make_argv_mock(std::vector<std::string> &string_vector){
            char ** cstrings = new char *[string_vector.size()+1];
            int pos = 0;
            for (auto &it : string_vector){
                cstrings[pos] = make_cstring(it);
                pos++;
            }
            cstrings[pos] = NULL;
            return cstrings;
        }

        //-Convert-string-to-non-const-cstring-------------------------------//
        char * make_cstring(std::string s){
            char * cstring = new char [s.length()+1];
            strcpy(cstring, s.c_str());
            return cstring;
        }

        int argc_mock = 0;
        std::vector<std::string> argv_mock_vec{};
        std::unique_ptr<char *> argv_mock;

};
