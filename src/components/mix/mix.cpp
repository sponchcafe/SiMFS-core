#include "shellutils/shellutils.hpp"

const std::string helpmessage = R"(.)";

class Mixer{

    public:
        Mixer(std::string n_output, std::vector<std::string> &n_inputs){
            output = sim::io::Output<sim::realtime_t>(n_output);
            for (auto &it: n_inputs){
                inputs.push_back(sim::io::Input<sim::realtime_t>(it));
            }
        }

        void mix(){
            std::sort(inputs.begin(), inputs.end());
            first = inputs.begin();
            second = first+1;
            while(inputs.size() > 1){
                while (first->peek() <= second->peek()){
                    if(!first->get(tag)){
                        std::swap(*inputs.begin(), *(inputs.end()-1));
                        inputs.pop_back();
                        break;
                    }
                    output.put(tag);
                }
                std::sort(inputs.begin(), inputs.end());
            }
            while(first->get(tag)){
                output.put(tag);
            }
        }

    private:
        std::vector<sim::io::Input<sim::realtime_t>> inputs;
        sim::io::Output<sim::realtime_t> output;
        sim::realtime_t tag;
        std::vector<sim::io::Input<sim::realtime_t>>::iterator first;
        std::vector<sim::io::Input<sim::realtime_t>>::iterator second;
};

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, {"SIM_PARAMS", "SIM_MIX_PARAMS"}};
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();

    //----------------------------------------------------------------------//
    std::vector<std::string> input_names = spec.get_option_list<std::string>
        (
         "inputs", {"i", "I"}, std::vector<std::string>{},
         "Input file names"
        );

    //----------------------------------------------------------------------//
    std::string output_name = spec.get_option<std::string>
        (
         "output", {"o", "O"}, "",
         "Output file name"
        );

    spec.enable_help(); // --help
    spec.enable_config(); // --config
    spec.enable_debug(); // --debug
    spec.enable_log(); // --log

    Mixer mixer(output_name, input_names);
    mixer.mix();
    
}
