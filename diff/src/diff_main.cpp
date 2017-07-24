#include <iostream>
#include "particle.h"
#include "getopt_pp.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;



int main(int argc, char* argv[]) {

	//json-file 
	std::ifstream ifs("jsonDiff.json", std::ifstream::in);
	if (!ifs) {
		std::cout << "file not found!" << std::endl;
	}
	json j; //json instance
	ifs >> j;

	//initialization
	uint64_t num;
	bool flag;
	double timeIncrementInSeconds;
	long int cylinderRadius;
	long int cylinderHalfHeight;
	double diffCoeff_nm;

	diffusion::Coordinate c;// { j["particle"]["coordinates"][0].get<int16_t>(), j["particle"]["coordinates"][1].get<int16_t>(), j["particle"]["coordinates"][2].get<int16_t>() }; //coordinate instance (diffusion coordinate)
	diffusion::particle p; //particle constructor

	//transfer parameters with getOpt ('call by char', "call by stream", parameter name, standart value)
	GetOpt::GetOpt_pp ops(argc, argv); //GetOpt instance
	ops >> GetOpt::Option('n', "numOfSteps", num, j["numOfSteps"].get<uint64_t>()); //60 s
	ops >> GetOpt::Option('d', "diffCoeff", diffCoeff_nm, j["particle"]["diffCoeff_nm"].get<double>());
	ops >> GetOpt::Option('t', "timeInc", timeIncrementInSeconds, j["particle"]["timeIncrementInSeconds"].get<double>());
	ops >> GetOpt::Option('r', "cylinderRadius", cylinderRadius, j["particle"]["cylinderRadius"].get<long int>());
	ops >> GetOpt::Option('h', "cylinderHalfHeight", cylinderHalfHeight, j["particle"]["cylinderHalfHeight"].get<long int>());

	/*ops >> GetOpt::Option('n', "numOfSteps", num, 600000000); //60 s
	ops >> GetOpt::Option('d', "diffCoeff", diffCoeff_nm, 4.35e+08);
	ops >> GetOpt::Option('t', "timeInc", timeIncrementInSeconds, 1e-07);
	ops >> GetOpt::Option('r', "cylinderRadius", cylinderRadius, 500L);
	ops >> GetOpt::Option('h', "cylinderHalfHeight", cylinderHalfHeight, 1000L);*/

	//set starting coordinates randomly
	std::uniform_int_distribution<int> dist_c_radius(0, cylinderRadius);
	std::uniform_real_distribution<double> dist_angle(0, 1);
	std::uniform_int_distribution<int> dist_c_half_height(-cylinderHalfHeight, +cylinderHalfHeight);

	int16_t initialRadialPos = dist_c_radius(p.random_engine);	
	double initialAngle = dist_angle(p.random_engine)*2*3.14159;

	int16_t x = std::cos(initialAngle)*initialRadialPos;
	int16_t y = std::sin(initialAngle)*initialRadialPos;	
	int16_t z = dist_c_half_height(p.random_engine);

	ops >> GetOpt::Option('x',  c.x, x);
	ops >> GetOpt::Option('y',  c.y, y);
	ops >> GetOpt::Option('z',  c.z, z);
	ops >> GetOpt::OptionPresent('b', flag);


	//particle-constructor instance
	diffusion::particle part = diffusion::particle(
													c,
													timeIncrementInSeconds,
													cylinderRadius,
													cylinderHalfHeight,
													diffCoeff_nm);


	
	//if flag true, write coordinates in binary representation 
	if (flag) {
		int16_t *_outdata = new int16_t[3]; /**< Intermediate storage for outgoing data. Stores the doubles of the coordinate struct to array elements. (This removes the defalt struct padding) */
		const char *_outbuf = reinterpret_cast<const char*>(_outdata); /**< Byte view of data array.  */

	for (uint64_t time = 0; time < num; time++) {
		//Diffusion-process
		part.move();
		_outdata[0] = part.getCurrentPositionX();
		_outdata[1] = part.getCurrentPositionY();
		_outdata[2] = part.getCurrentPositionZ();
		std::cout.write(_outbuf, sizeof(int16_t)*3);
		}
}
else { //if flag false then write coordinates in standart output stream
	for (uint64_t time = 0; time < num; time++) {
		//Diffusion-process
		part.move();
		std::cout
		<< "X-Coordinate in nm: " << part.getCurrentPositionX() << ","
		<< " Y-Coordinate in nm: " << part.getCurrentPositionY() << ","
		<< " Z-Coordinate in nm: " << part.getCurrentPositionZ() << ","
		<< std::endl;
	}
}

	return 0;
}
