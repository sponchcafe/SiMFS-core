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

typedef struct{
    //std::string name;
    //std::ifstream stream;
    sim::io::Input<sim::io::timetag> *in;
    sim::io::timetag tag;
} input;

bool comp(input &i1, input &i2){
    return (i1.tag < i2.tag);
}

void swap(std::vector<input> &iv, int i, int j){
    auto it = iv.begin();
    input temp;
    temp = std::move(*(it+i));
    *(it+i) = std::move(*(it+j));
    *(it+j) = std::move(temp);
}


// sort the first element into the vector util it is sorted again
void bubble_up(std::vector<input> &iv){
    auto it = iv.begin();
    input temp;
    while((it+1) < iv.end() && !comp(*it, *(it+1))){
        temp = std::move(*it);
        *it = std::move(*(it+1));
        *(it+1) = std::move(temp);
        ++it;
    }
}

int main (int argc, char *argv[]){

    sim::opt::Parameters p{argc, argv, "mix"};
    std::string out_filename = p.getOption('o', "output", sim::opt::empty);
    std::vector<input> inputs;
    std::vector<std::string> names;

    *(p.ops) >> GetOpt::GlobalOption(names);
    
    p.enableConfig();
    p.enableHelp(helpmessage);

    sim::io::Output<sim::io::timetag> output(out_filename);

    for (auto it = names.begin(); it != names.end(); ++it){
        inputs.push_back(input{});
        auto last = inputs.end()-1;
        //last->name = *it;
        //last->stream = std::ifstream(*it);
        last->in = new sim::io::Input<sim::io::timetag>(*it);
        last->in->get(last->tag);
    }

    // sort the inputs according to the current timetag
    std::sort (inputs.begin(), inputs.end(), comp);

    while(true){

        auto first = inputs.begin();
        auto second = first+1;

        while(first->tag <= second->tag){

            //sim::io::write_binary(std::cout, first->tag);
            output.put(first->tag);

            //if(!sim::io::read_binary(first->stream, first->tag)){
            if(!first->in->get(first->tag)){
                swap(inputs, 0, inputs.size()-1);

                inputs.pop_back();
                if (inputs.size() <= 1) {
                    // final pass through of the last stream
                    do{
                        //sim::io::write_binary(std::cout, first->tag);
                        output.put(first->tag);
                    //}while(sim::io::read_binary(first->stream, first->tag));
                    }while(first->in->get(first->tag));
                    for (auto it=inputs.begin(); it!=inputs.end(); ++it){
                        delete(it->in);
                    }
                    exit(EXIT_SUCCESS);
                }
            }
        }
        // reestablishing the invariant: inputs are sorted in the input vector
        bubble_up(inputs);
    }

}