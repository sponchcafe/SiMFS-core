#include <iostream>
#include "diffusion/component.hpp"
#include "io/file_io.hpp"
#include <sstream>

using namespace sim;

int main(int argc, char *argv[]) {

    //-Create----------------------------------------------------------------//
    comp::Diffusion dif;

    //-Get-parameters--------------------------------------------------------//
    std::stringstream ss; std::string s;
    while (std::getline(std::cin, s)) ss << s;
    json j; ss >> j;

    //-Configure-------------------------------------------------------------//
    dif.set_json(j);
    dif.set_coordinate_output<file_io::FileOutput>();
    dif.set_collision_output<file_io::FileOutput>();

    //-Initialize------------------------------------------------------------//
    dif.init();

    //-Log-------------------------------------------------------------------//
    j = dif.get_json();
    std::cout << j << std::endl;

    //-Run-------------------------------------------------------------------//
    dif.run();

}
