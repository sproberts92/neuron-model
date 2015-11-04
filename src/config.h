#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include "libconfig.h"

typedef struct{
	int dim, n_neurons, growth_iter, prop_iter;
	double schwann_l;
}user_config_t;

void getConfigInfo(user_config_t *config);

#endif
