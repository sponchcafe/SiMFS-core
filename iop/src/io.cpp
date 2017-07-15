#include "io.hpp"

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