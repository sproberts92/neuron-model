#include "rand_gen.h"

void rand_gen::seed_gen(void) 
{
	auto timePoint = std::chrono::system_clock::now();
	auto init_seed = static_cast<rType::result_type>(timePoint.time_since_epoch().count());

	generator.seed(init_seed);
}

double rand_gen::get_rnum(void)
{
	return (double) udist(generator);
}