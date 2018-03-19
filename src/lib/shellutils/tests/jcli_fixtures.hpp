#include "gtest/gtest.h"
#include "shellutils/shellutils.hpp"
    

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
void write_json_file(json object, std::string filename){
    std::ofstream outfile;
    outfile.open(filename);
    outfile << object.dump(4) << std::endl;
    outfile.close();
}


//---------------------------------------------------------------------------//
json read_json_file(std::string filename){
    json object;
    std::ifstream infile;
    infile.open(filename);
    try{
        infile >> object;
    } catch (std::exception e){}
    infile.close();
    return object;
}

//---------------------------------------------------------------------------//
class JsonCliTest: public ::testing::Test{

    protected:

        //-------------------------------------------------------------------//
        JsonCliTest() {}
        
        //-------------------------------------------------------------------//
        virtual void SetUp() {

            json env = {
                { 
                    "my_scope", {
                        {"along", "enva"},
                        {"blong", "envb"},
                        {"clong", "envc"},
                        {"dlong", "envd"}
                    }
                }
            };

            write_json_file(env, "env.json");

            json args = {
                {
                    "my_scope", {
                        {"along", "arga"},
                        {"blong", "argb"},
                        {"clong", "argc"}
                    }
                }
            };

            write_json_file(args, "args.json");

            json include = {
                {"somekey", "someval"}
            };

            write_json_file(include, "include.json");

            std::vector<std::string> argv_vec = {
                "./execname", "@args.json",
                "-a", "shella",
                "--blong", "shellb", 
                "--multi", "1", "2", "3",
                "--invalid", "1", "2", "string",
                "--invalid2", "1", "2", "string", "3",
                "--parse", "{\"a\":-6}",
                "--include", "@include.json",
                "--scope", "/my_scope"
                
            };

            std::vector<std::string> envp_vec = {
                "SIM_PARAM_FILE=@env.json"
            };

            int argc = argv_vec.size();
            char ** argv = make_mock_argv(argv_vec);
            char ** envp = make_mock_argv(envp_vec);

            cli.reset(new jcli::JsonCli(argc, argv, envp,
                    {"SIM_PARAM_FILE", "SIM_SCOPE_PARAMFILE"}));

            spec.reset(new jcli::JsonSpec(*cli));
            spec->enable_scoping();
            spec->enable_debug();
            spec->enable_config();
            spec->enable_help();

        }

        //-------------------------------------------------------------------//
        virtual void TearDown() {
            std::remove("env.json");
            std::remove("args.json");
            std::remove("include.json");
        }

        std::unique_ptr<jcli::JsonCli> cli;
        std::unique_ptr<jcli::JsonSpec> spec;

};

