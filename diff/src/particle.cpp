#include "particle.h"
#include <iostream>

namespace diffusion {

	particle::particle(){

		//this->timeIncrementInSeconds;
		//this->cylinderRadius;
		//this->cylinderHalfHeight;
		//this->diffCoeff_nm;
		//this->standardDeviation = sqrt(2 * this->diffCoeff_nm*this->timeIncrementInSeconds);
		//this->radialPos = 0;
	}

	particle::particle(
		Coordinate coordinate,
		double timeIncrementInSeconds,
		long int cylinderRadius,
		long int cylinderHalfHeight,
		double diffCoeff_nm) {

		this->coordinate = { coordinate.x, coordinate.y, coordinate.z };
		this->timeIncrementInSeconds = timeIncrementInSeconds;
		this->cylinderRadius = cylinderRadius;
		this->cylinderHalfHeight = cylinderHalfHeight;
		this->diffCoeff_nm = diffCoeff_nm;
		this->standardDeviation = sqrt(2 * this->diffCoeff_nm*this->timeIncrementInSeconds);
		this->radialPos = 0;

	}

	particle::~particle() {};

	void particle::move() {

		int16_t stepX = (int16_t) this->standardDeviation*((double)norm_dist(random_engine));
		int16_t stepY = (int16_t) this->standardDeviation*((double)norm_dist(random_engine));
		int16_t stepZ = (int16_t) this->standardDeviation*((double)norm_dist(random_engine));

		this->coordinate.x += stepX;
		this->coordinate.y += stepY;
		this->coordinate.z += stepZ;

		this->radialPos = std::sqrt(this->coordinate.x * this->coordinate.x + this->coordinate.y * this->coordinate.y);

		if (this->radialPos > cylinderRadius) {

			this->coordinate.x -= stepX;
			this->coordinate.y -= stepY;
			this->radialPos = std::sqrt(this->coordinate.x * this->coordinate.x + this->coordinate.y * this->coordinate.y);

		}
		if (this->coordinate.z < -cylinderHalfHeight || this->coordinate.z > cylinderHalfHeight)
			this->coordinate.z -= stepZ;

	}




	int particle::setCurrentPositionX(int newCurrentPositionX) {

		int newRadialPosition = std::sqrt(newCurrentPositionX*newCurrentPositionX + this->coordinate.y*this->coordinate.y);
		if (newRadialPosition > cylinderRadius)
			return 1;	//invalid x-coordinate
		else {
			this->coordinate.x = coordinate.x;
			this->radialPos = newRadialPosition;
			return 0;
		}

	}

	int particle::setCurrentPositionY(int newCurrentPositionY) {

		int newRadialPosition = std::sqrt(std::pow(this->coordinate.x, 2.0) + std::pow(newCurrentPositionY, 2.0));
		if (newRadialPosition > cylinderRadius)
			return 1;
		else {
			this->coordinate.y = coordinate.y;
			this->radialPos = newRadialPosition;
			return 0;
		}

	}

	int particle::setCurrentPositionZ(int currentPositionZ) {

		if (std::abs(coordinate.z) > std::abs(cylinderHalfHeight))
			return 0;
		else {
			this->coordinate.z = currentPositionZ;
			return 1;

		}


	}


	int16_t particle::getCurrentPositionX() {

		return  this->coordinate.x;

	}

	int16_t particle::getCurrentPositionY() {

		return  this->coordinate.y;

	}

	int16_t particle::getCurrentPositionZ() {

		return this->coordinate.z;

	}
}