#include <limits>
#include <iostream>
#include <cmath>
#include <sim/fluorophore.hpp>
#include <sim/constants.hpp>

namespace ph2{

    /** On construction, the global clock, dwell clock and dwell time are set to 0, the random generator
     * is initialized with the seed. If `text` is true, the fluorophore's emitter is initialized to a new
     * ph2::text_stream_emitter, that writes to the specified output stream. Otherwise, a ph2::binary_stream_emitter
     * is used. */
    fluorophore::fluorophore(unsigned seed, double dwell, sim::io::Output<sim::io::timetag> &out)
        : global_clock(0), dwell_clock(0), dwell_time(dwell), output(out)
    {
        init_random(seed);
    }


    /** States are sequentially added to an internal states vector. Each state is initialized with an empty paths vector.*/
    void fluorophore::add_state(std::string n)
    {
        states.push_back(state{n, std::vector<path>{}});
    }

    /** If the named state is not found, the current state is set to `nullptr`. */
    void fluorophore::set_current_state(std::string n)
    {
        current = name_to_address(n);
    }

    /** Connecting two states adds a new path to the outgoing paths vector of the source state. It stores the name of the target state,
     * a rate constant, an emission state and a pointer to a ph2::base_rate_generator. Note however, that the pointer to the target
     * state is not set until the ph2::fluorophore::initialize is called.*/
    void fluorophore::connect_states(std::string n1, std::string n2, double rate, bool emit, double epsilon)
    {
        state* const s1 = name_to_address(n1);
        path p = path{n2, rate, nullptr, emit, epsilon};
        s1->paths.push_back(p);        
    }

    /** Reconnects all states, i.e. recalculates all pointers of outgoing paths to their respective target states and collects all 
     * paths with position dependent rates for faster rate updates. The current state is set to the state named n. */
    void fluorophore::initialize(std::string n)
    {
        reconnect_all();
        assign_dyn_paths();
        set_current_state(n);
    }

    /** Iterates over all paths stored in the dyn_paths vector and calls their rate generators with the new coordinate. The results
     * are stored in the paths rate field for usage in the simulation. Furthermore the dwell time for the next diuffsion step is extracted
     * from the coordinate. */
    void fluorophore::update_environment(sim::io::flux &f, sim::io::efficiency &e)
    {
        detection_efficiency = e;
        for (auto it = dyn_paths.begin(); it != dyn_paths.end(); ++it){
            path *p = *it;
            p->rate = f * p->epsilon * 100.0/1000 * log(10) / CONST_NA;
        };
    }

    /** Simulation loop for one diffusion step. 
     * ### Description:
     * 1. Get lifetimes for all outgoing states
     * 2. Get the minimum of all lifetimes
     * 3. If the lifetime is within the current dwelltime (a), progress to the next state, else (b), set the global clock to the end of the dwell period, reset the dwell_clock and return
     * 4. Add the lifetime to the global and the local (dwell-) clock 
     * 5. If the traversed path is an emission path, call the emitter with the current global time
     * 6. Continue at 1. with the new current state */
    void fluorophore::simulate()
    {

        while(1){

            path* path_to_next;
            double min_lifetime = std::numeric_limits<double>::max();
            double lifetime = std::numeric_limits<double>::max();

            // find the minimal lifetime (1./2.)
            for (auto p_it = current->paths.begin(); p_it != current->paths.end(); ++p_it){

                lifetime = get_lifetime(p_it->rate);

                if (lifetime < min_lifetime){
                    min_lifetime = lifetime;
                    path_to_next = &(*p_it);
                }

            }

            // step (3a.)
            if (dwell_clock + min_lifetime < dwell_time){

                // (4.)
                global_clock += min_lifetime;
                dwell_clock += min_lifetime;

                // emission? (5.)
                if (path_to_next->emit && ( (double) random_engine() / (double) random_engine.max()) < detection_efficiency){
                    sim::io::timetag t = global_clock;
                    output.put(t);
                }

                // go to next state (3a.)
                current = path_to_next->target;

            }

            // terminate simulation loop (3b.)
            else{
                global_clock += (dwell_time-dwell_clock);
                dwell_clock = 0;
                break;
            }

        }

    }

    /** Using `std::mt19937` random generator initialized with a given seed. */
    void fluorophore::init_random(unsigned seed)
    {
        random_engine = std::mt19937 (seed);
    }

    /** Prints the states and their connections with rate and emission information.
     * For each state **S** the following output will be generated:
     *
     * >  S0: -> S1(~<rate>*) ...
     * 
     * The initial > is set only for the current state. For each connection -> XX, the rate constant is printed in parentheses.
     * A '~' preceeding a rate indicates a position dependent rate, a subsequen '*' denotes an emission path.
     * 
     * *TODO:* In the future this function should return the info string rather than printing it directly. */
    const std::string fluorophore::get_info_string() const
    {
        for (auto s_it = states.begin(); s_it != states.end(); ++s_it){
            std::clog << ((current->name == s_it->name) ? ">":" ") << s_it->name << ": ";
            for (auto p_it = s_it->paths.begin(); p_it != s_it->paths.end(); ++p_it){
                std::clog << "->" << p_it->target_name << " (" << ((p_it->epsilon != 0) ? "~":" ") << p_it->rate << ((p_it->emit) ? "*":" ") << ") ";
            }
            std::clog << std::endl;
        }

        // TODO
        return "...";
    }

    /** Get the address of a state in the state vector from its name by linear search. The address is only valid as long as there is no
     * reallocation due to the addition of new states. If the state name is not found, `nullptr` is returned.*/
    fluorophore::state* fluorophore::name_to_address(std::string n)
    {
        for (auto it = states.begin(); it != states.end(); ++it){
            if(it->name == n){
                return &(*it);
            }
        }
        return nullptr;
    }

    /** Reconnect all states by iterating over all paths and reacalculating the addresses of the target states from the paths' target names.*/
    void fluorophore::reconnect_all()
    {
        for (auto s_it = states.begin(); s_it != states.end(); ++s_it){
            for (auto p_it = s_it->paths.begin(); p_it != s_it->paths.end(); ++p_it){
                p_it->target = name_to_address(p_it->target_name);
            }
        }
    }

    /** Get all paths with non-`nullptr` rate generators and push them to the cleared dyn_paths vector. */
    void fluorophore::assign_dyn_paths()
    {
        dyn_paths.clear();
        for (auto s_it = states.begin(); s_it != states.end(); ++s_it){
            for (auto p_it = s_it->paths.begin(); p_it != s_it->paths.end(); ++p_it){
                if (p_it->epsilon > 0){
                    dyn_paths.push_back( &(*p_it) );
                }
            }
        }
    }


    /** The lifetimes are generated from a raw 32bit random number. 
     *  If lambda (rate) is 0, a maximum double value is returned.
     * The ratio y of the random number and the maximal random number yields a double between 0 and 1.
     * Inversion of the exponential distribution : \f$-frac{\ln(1-y)}{\lambda}\f$ yields an exponentially distributed lifetime. */
    const double fluorophore::get_lifetime(double lambda)
    {
        if (lambda <= 0){
            return std::numeric_limits<double>::max();
        }

        double y = ( (double) random_engine() / (double) random_engine.max());
        return -log(1-y) / lambda;
    }

}
