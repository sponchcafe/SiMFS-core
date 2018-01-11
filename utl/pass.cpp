#include "sim_util.hpp"

const std::string helpmessage = 
R"(
TODO;
    [pass]

    -i --input : Input file (coordinates), defaults to standard in.
    -o --output : Output file (flux), defaults to standard out.

    -b --buffer-size : Buffer size in bytes.
    -t --type : Data type, one of ([c]oordinate, [t]imetag, [f]lux, [e]fficiency, [p]hoton_count), default is [t]imetag.
)";
int main (int argc, char *argv[]){

    size_t buffer_size = 0;
   
    sim::opt::Parameters p{argc, argv, "pass"};
    std::string in_filename = p.getOption('i', "input", sim::opt::empty);
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
	buffer_size = p.getOption('b', "buffer-size", 1024);
	
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Input<sim::io::timetag> input(in_filename);
    sim::io::Output<sim::io::timetag> output(out_filename);
    sim::io::timetag data;
    input.get(data);
    do{
        output.put(data);
    }while(input.get(data));
}