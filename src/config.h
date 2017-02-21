#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include "libconfig.h"

/* This module is a bit messy, but I intend to replace libconfig with a JSON
 * based solution such as YAJL so there isn't much point in tidying it up at
 * the moment (see branch json-config for the start of this conversion). */

typedef struct{
	const char *dir, *name, *ext;
}path_t;

typedef struct{
	int dim, n_neurons, growth_iter, prop_iter, target_age, combination_rule;
	double schwann_l, link_fwhm_param, *bounds;
	path_t growth, network_c, network_n, network_r, signal_prop, neuron_adj, activity;
}user_config_t;

void getConfigInfo(user_config_t *config);
void check_lookup(config_setting_t *setting);
void check_val_lookup(int lookup_success);

#endif
