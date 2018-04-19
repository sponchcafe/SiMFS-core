#include "shellutils/shellutils.hpp"
#include "graph/main.hpp"
#include "random/random.hpp"

const std::string helpmessage = R"(.)";

class Splitter{

    public:

        Splitter(
                std::string photon_input_name, 
                std::string efficiency_input_name, 
                std::string output_accepted_name,
                std::string output_rejected_name, 
                unsigned seed) 
            :
                photon_input{photon_input_name},
                efficiency_input{efficiency_input_name},
                output_accepted{output_accepted_name},
                output_rejected{output_rejected_name},
                uni{seed}
        {
        }

        void run() {

            sim::realtime_t timetag{0.0};
            sim::TimedValue efficiency{0.0, 0.0};
            efficiency_input.get(efficiency);

            while(photon_input.get(timetag)){
                while(timetag > efficiency_input.peek().time){
                    if (!efficiency_input.get(efficiency)) break;
                }
                if (uni() < efficiency.value){
                    output_accepted.put(timetag);
                }
                else {
                    output_rejected.put(timetag);
                }
            }
            while(efficiency_input.get(efficiency));

        }

    private:
        sim::io::Input<sim::realtime_t> photon_input;
        sim::io::Input<sim::TimedValue> efficiency_input;
        sim::io::Output<sim::realtime_t> output_accepted;
        sim::io::Output<sim::realtime_t> output_rejected;
        sim::random::Uniform uni;

};

int main(int argc, char **argv, char **envp){

    //----------------------------------------------------------------------//
    jcli::JsonCli cli{argc, argv, envp, {"SIM_PARAMS", "SIM_SPL_PARAMS"}};
    jcli::JsonSpec spec{cli};

    spec.enable_scoping();

    //----------------------------------------------------------------------//
    std::string photon_input_name = spec.get_option<std::string>
        (
         "photon-input", {"p", "P"}, "",
         "Photon input stream"
        );

    //----------------------------------------------------------------------//
    std::string efficiency_input_name = spec.get_option<std::string>
        (
         "efficiency-input", {"e", "E"}, "",
         "Efficiency input stream"
        );

    //----------------------------------------------------------------------//
    std::string output_accepted_name = spec.get_option<std::string>
        (
         "output-accepted", {"a", "A"}, "",
         "Output stream for accepted photons"
        );

    //----------------------------------------------------------------------//
    std::string output_rejected_name = spec.get_option<std::string>
        (
         "output-rejected", {"r", "R"}, "",
         "Output stream for rejected photons"
        );
    
    //----------------------------------------------------------------------//
    unsigned seed = spec.get_option<unsigned>
        (
         "seed", {"s", "S"}, sim::random::get_new_seed(),
         "Random seed"
        );

    spec.enable_help(); // --help
    spec.enable_config(); // --config
    spec.enable_debug(); // --debug
    spec.enable_log(); // --log

    Splitter split(
            photon_input_name, 
            efficiency_input_name, 
            output_accepted_name,
            output_rejected_name, 
            seed
            );

    split.run();
    
}
