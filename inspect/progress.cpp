#include "sim_util.hpp"

const std::string helpmessage = 
R"(
)";

int main (int argc, char *argv[]){

    sim::io::coordinate c{0,0,0};
    long unsigned int i = 0;
    while(sim::io::read_binary(std::cin, c)){
        i++;
        sim::io::write_binary(std::cout, c);
        if ((i%1000000) == 0){
            std::clog << i/1e+7 << " s\r";
        }
    }

}