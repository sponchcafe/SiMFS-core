#ifndef PH2_FLUOROPHORE_H
#define PH2_FLUOROPHORE_H

#include <string>
#include <vector>
#include <iostream>
#include <random>

#include "sim_util.hpp"
 
namespace ph2{

    /** Simulator class to model the photophysics within a fluorophore based on state transitions with constant transition rates.*/
    class fluorophore{

        public:

            /** Constructor
                * @param
                * [in]
                * - **seed:** seed for random number generation*/
            fluorophore(unsigned seed, double dwell, std::ostream &out);

            /** Add a named state to the fluorophore.
                * @param
                * [in]
                * - **n:** name of the new state
                */
            void add_state(std::string n);

            /** Set the fluorophores current state.
                * @param
                * [in]
                * - **n:** name of the state to set the fluorophore into */
            void set_current_state(std::string n);

            /** Connect two existing states and define the transition between them.
                * @param
                * [in] 
                * - **n1:** name of the source state
                * - **n2:** name of the target state
                * - **rate:** transition rate constant used for lifetime generation
                * - **emit:** if true, traversal of this path triggers a call to the fluorophore's emitter
                * - **epsilon:** absorption coefficient*/
    
            void connect_states(std::string n1, std::string n2, double rate, bool emit, double epsilon);

            /** Initialize the fluorophore
                * @param
                * [in]
                * - **n:** name of the initial state
                *
                * Sets the initial state of the fluorophore and recalculates all connections in the fluorophore state diagram.
                * 
                * **Note: Needs to be called whenever the state diagram is altered!** */
            void initialize(std::string n);

            /** Move the fluorophore to a given coordinate and update all dynamic rates.
                * @param
                * [in]
                * - **c:** coordinate to move to
                */
            void update_environment(sim::io::flux &f, sim::io::efficiency &e);

            /** Core simulation routine.
            */
            void simulate();

            /** Print an info string to the standard output
                *Debugging only* */ 
            const std::string get_info_string() const;

        private:
    
            struct state;
            
            /** Representation of a path in the Jablonsky diagram */
            struct path{
            	std::string target_name;        /**< name of the target state*/
            	double rate;                    /**< rate constant of transition*/
            	state *target;                  /**< pointer to target state*/
            	bool emit;                      /**< true if emission path*/
                double epsilon;                 /**< absorption coefficient*/
            };

            /** Representation of a state in the Jablonsky diagram */
            struct state{
            	std::string name;           /**< name of the state*/
            	std::vector<path> paths;    /**< list of outgoing paths*/
            };
            
            /** States of the fluorophore */
		    std::vector<state> states;

            /** Pointer to the current state */
            state *current;
		    
            /** List of pointers to all states that have position dependent rates. */
            std::vector<path*> dyn_paths;
         
            /** Random engine for lifetime calculation. */
            std::mt19937 random_engine;


            double detection_efficiency;

		    /** Real time of simulation. */
		    double global_clock;

            /** Time within the current diffusion step. */
		    double dwell_clock;

            /** Time to simulate until the next diffusion step. */
            double dwell_time;

            std::ostream &output;

		    /** Get the address of a state by its name. */
		    state* name_to_address(std::string n);

            /** Initilaize the random engine with a given seed. */
            void init_random(unsigned seed);

            /** Recalculate all connections in the Jablonsky diagram. */
            void reconnect_all();

            /** Collect all paths with position dependent rates to the dyn_paths vector. */
            void assign_dyn_paths();

            /** Gat a random lifetime based on the mean rate lambd. */
            const double get_lifetime(double lambda);
         
    };

}

#endif