#include "sim_util.hpp"
#include <algorithm>

const std::string helpmessage = 
R"(
Mix multiple photonstreams into one.
Photons of each input stream are sorted online and pushe to the standard output.

Usage: mix <file1 file2 ...>

    [mix]
    
    -o --output : Outputfile (timtags), defaults to standard out.

)";

using mix_input_t = sim::io::Input<sim::io::timetag>;
using mix_output_t = sim::io::Output<sim::io::timetag>;

int main (int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "mix"};
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    
    // get the input arguments as file names and create an sim::io::output
    std::vector<std::string> names;
    *(p.ops) >> GetOpt::GlobalOption(names);
    mix_output_t mix_output(out_filename);
    
    p.enableConfig();
    p.enableHelp(helpmessage);

    std::vector<mix_input_t> input_handlers;
    for (auto it = names.begin(); it != names.end(); ++it){
        input_handlers.push_back(mix_input_t(*it));
    }

    std::sort(input_handlers.begin(), input_handlers.end());

    auto first = input_handlers.begin();
    auto second = (first+1);

    sim::io::timetag current_tag;
    
    while(input_handlers.size() > 1){
        while (first->peek() <= second->peek()){
            if(!first->get(current_tag)){
                std::swap(*input_handlers.begin(), *(input_handlers.end()-1));
                input_handlers.pop_back();
                break;
            }
            mix_output.put(current_tag);
            
        }
        std::sort(input_handlers.begin(), input_handlers.end());
    }

    while(first->get(current_tag)){
        mix_output.put(current_tag);
    }

}