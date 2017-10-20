#include "sim_util.hpp"

namespace sim{

    namespace log{
    
        void info(std::string msg) {
            std::clog << msg;
        }
        void warn(std::string msg) {
            std::cerr << msg;
        }
        void critical(std::string msg) {
            std::cerr << msg; exit(EXIT_FAILURE);
        }
    };

    namespace opt{

        Parameters::Parameters(int argc, char* argv[], const std::string rootname) : rootname(rootname){
            ops = new GetOpt::GetOpt_pp(argc, argv, GetOpt::Include_Environment);
            filename = ops->getopt<std::string>('p', "PARAMETERS", "params.json");
            std::ifstream param_file(filename, std::ifstream::in);
            
            if (param_file) {
                param_file >> params;
                params = params[rootname];
            }
        }
        
        void Parameters::enableConfig(bool terminate){
            bool config = false;
                *ops >> GetOpt::OptionPresent('c', "config", config);
                if (config){
                    json config_param;
                    config_param[rootname] = params;
                    std::cout << config_param.dump(4);
                    if (terminate){
                        std::cout << std::endl;
                        exit(EXIT_SUCCESS);
                    }else{
                        std::cout << "," << std::endl;
                    }
                }   
        }
        
        void Parameters::enableHelp(std::string helpmessage){
            bool help = false;
                *ops >> GetOpt::OptionPresent('h', "help", help);
                if (help){
                    log::info(helpmessage);
                    log::info(opt_helpmessage);
                    exit(EXIT_SUCCESS);
                }
        }

    };

};
