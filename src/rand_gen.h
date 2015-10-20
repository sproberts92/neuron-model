#pragma once

#include <random>
#include <chrono>

class rand_gen
{
private:
	typedef std::mt19937 rType;
	rType generator;
	std::uniform_real_distribution<double> udist;

public:
	rand_gen(double rMin = 0, double rMax = 100) : udist(rMin, rMax)
	{	
		this->seed_gen(); 
	}

	void seed_gen(void);
	double get_rnum(void);
};