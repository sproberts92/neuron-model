#pragma once

#include <random>
#include <chrono>

template<class T>
struct Distribution{};

/* Need to specialise becuase we use different distributions for ints
 * and floats/doubles. This could probably also be generalised for 
 * different distributions entirely, eg gaussian. */
template<>
struct Distribution<int>{
	typedef std::uniform_int_distribution<int> type;
};

template<>
struct Distribution<float>{
	typedef std::uniform_real_distribution<float> type;
};

template<>
struct Distribution<double>{
	typedef std::uniform_real_distribution<double> type;
};

template <typename T> class rand_gen
{
private:
	typedef std::mt19937 rType;
	rType generator;
	typename Distribution<T>::type udist;

public:
	rand_gen(T rMin = 0, T rMax = 100) : udist(rMin, rMax)
	{	
		this->seed_gen(); 
	}

	void seed_gen(void);
	T get_rnum(void);
};

template <typename T> void rand_gen<T>::seed_gen(void) 
{
	auto timePoint = std::chrono::system_clock::now();
	auto init_seed = static_cast<rType::result_type>(timePoint.time_since_epoch().count());

	generator.seed(init_seed);
}

template <typename T> T rand_gen<T>::get_rnum(void)
{
	return (T)udist(generator);
}