#include "particle.hpp"
#include <cmath>

const std::string helpmessage = 
R"(
Create a random diffusion path as coordinates written in binary 16-bit integers to the standard output.
The coordinates are part of a cylindrical space.

Usage: dif [options]

	-n --steps : Number of steps to simulate.
	-i --increment : Time increment in seconds.
	-r --radius : Radius of the cylinder.
	-l --half-height : Half height of the cylinder.
	-d --diff-coef : Diffusion coefficient.
	-s --seed : Random seed.

	-p --PARAMETERS : Specify a parameter json file. Defaults to PARAMETERS environment variable or 'defaults.json'.
	-c --config : Generate a config .json file and print it to the standard output.
	-h --help : Show this help message.

)";

int main (int argc, char *argv[]){

 	sim::opt::Parameters p{argc, argv, "dif"};
	double increment = p.getOption('i', "increment", 1e-7);
	long radius = (long) (1e+9*p.getOption('r', "radius", 500e-9));
	long half_height = (long) (1e+9*p.getOption('l', "half-height", 1000e-9));
	double diff_coef = p.getOption('d', "diff-coef", 1e+8);
	uint64_t steps = p.getOption('n', "steps", 10000000);
	unsigned int seed = p.getOption('s', "seed", 0);
	p.enableConfig();
	p.enableHelp(helpmessage);
	
	//set starting coordinates randomly
	std::uniform_int_distribution<int> dist_c_radius(0, radius);
	std::uniform_int_distribution<int> dist_c_half_height(-half_height, +half_height);
	std::uniform_real_distribution<double> dist_angle(0, 1);


	sim::io::coordinate c{0, 0, 0};
	diffusion::particle particle = diffusion::particle(c, increment, radius, half_height, diff_coef, seed);
	int16_t initialRadialPos = dist_c_radius(particle.random_engine);
	double initialAngle = dist_angle(particle.random_engine)*2*CONST_PI;

	c.x = (int16_t) (std::cos(initialAngle) * initialRadialPos);
	c.y = (int16_t) (std::sin(initialAngle) * initialRadialPos);	

	c.z = dist_c_half_height(particle.random_engine);

	particle.coordinate.x = c.x;
	particle.coordinate.y = c.y;
	particle.coordinate.z = c.z;

	//Diffusion-process
	for (uint64_t step = 0; step < steps; step++) {
		c.x = particle.getCurrentPositionX();
		c.y = particle.getCurrentPositionY();
		c.z = particle.getCurrentPositionZ();
		sim::io::write_binary(std::cout, c);
		particle.move();
	}

	return 0;

}
