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

    namespace io{

        std::ostream& write_binary(std::ostream& os, const coordinate& c){
            const char *out_buffer = reinterpret_cast<const char *>(&c);
            return os.write(out_buffer, sizeof(coordinate));
        }

        std::istream& read_binary(std::istream &is, coordinate& c){
            char *in_buffer = reinterpret_cast<char *>(&c);
            is.read(in_buffer, sizeof(coordinate));
                if (!is){
                    is.setstate(std::ios_base::failbit);
                return is;
            }
            return is;
        }

        std::ostream& write_binary(std::ostream& os, const double& x){
            const char *out_buffer = reinterpret_cast<const char *>(&x);
            return os.write(out_buffer, sizeof(double));
        }

        std::istream& read_binary(std::istream &is, double& x){
            char *in_buffer = reinterpret_cast<char *>(&x);
            is.read(in_buffer, sizeof(double));
                if (!is){
                    is.setstate(std::ios_base::failbit);
                return is;
            }
            return is;
        }

    }

    namespace opt{

        Parameters::Parameters(int argc, char* argv[], const std::string rootname) : rootname(rootname){
            ops = new GetOpt::GetOpt_pp(argc, argv, GetOpt::Include_Environment);
            filename = ops->getopt<std::string>('p', "PARAMETERS", "default.json");
            std::ifstream param_file(filename, std::ifstream::in);
            
            if (param_file) {
                param_file >> params;
                params = params[rootname];
            }
        }
        
        void Parameters::enableConfig(){
            bool config = false;
                *ops >> GetOpt::OptionPresent('c', "config", config);
                if (config){
                    json config_param;
                    config_param[rootname] = params;
                    std::cout << config_param.dump(4);
                    exit(EXIT_SUCCESS);
                }
        }
        
        void Parameters::enableHelp(std::string helpmessage){
            bool help = false;
                *ops >> GetOpt::OptionPresent('h', "help", help);
                if (help){
                    log::info(helpmessage);
                    exit(EXIT_SUCCESS);
                }
        }

    };

};
