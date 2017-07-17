#ifndef PARTICLE_H
#define PARTICLE_H

#include <random>
#include <iostream>
#include "sim_util.hpp"

namespace diffusion {

	class particle {

	public:
		//Konstruktor:
		particle();
		particle(
			sim::io::coordinate coordinate,
			double timeIncrementInSeconds,
			long int cylinderRadius,
			long int cylinderHalfHeight,
			double diffCoeff_nm);
		//Dekonstruktor:
		~particle();

		//Methoden:
		void move();
		int setCurrentPositionX(int position);
		int setCurrentPositionY(int position);
		int setCurrentPositionZ(int position);
		int16_t getCurrentPositionX();
		int16_t getCurrentPositionY();
		int16_t getCurrentPositionZ();

		//Attribute:
		sim::io::coordinate coordinate;
		double timeIncrementInSeconds;
		long int cylinderRadius;
		long int cylinderHalfHeight;
		double diffCoeff_nm;
		double standardDeviation;
		std::mt19937 random_engine;
		std::normal_distribution<> norm_dist;
		long int radialPos;
	};
}

#endif
