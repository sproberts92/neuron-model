#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include "libconfig.h"

typedef struct{
	const char *dir, *name, *ext;
}path_t;

typedef struct{
	int dim, n_neurons, growth_iter, prop_iter, target_age;
	double schwann_l, link_fwhm_param, *bounds;
	path_t growth, network_c, network_n, network_r, signal_prop, neuron_adj, activity;
}user_config_t;

void getConfigInfo(user_config_t *config);
void check_lookup(config_setting_t *setting);
void check_val_lookup(int lookup_success);

#endif
