#pragma once


#include <random>
#include <iostream>

namespace diffusion {

	typedef struct coordinate {
		int16_t x;
		int16_t y;
		int16_t z;
	} Coordinate;

	class particle {

	public:
		//Konstruktor:
		particle();
		particle(
			Coordinate coordinate,
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
		Coordinate coordinate;
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
