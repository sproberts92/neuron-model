#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "yajl/yajl_tree.h"

typedef struct{
	const char *dir, *name, *ext;
}path_t;

typedef struct{
	int dim, n_neurons, growth_iter, prop_iter;
	double schwann_l, link_fwhm_param, *bounds;
	path_t growth, network_c, network_r, signal_prop, neuron_adj, activity;
}user_config_t;

int check_parse_error(yajl_val node, char *errbuf, int v);
char *read_file(long int *length, const char *file_name);
int read_config(user_config_t *cf);
int get_json_int(yajl_val *node, char *name[]);
double get_json_double(yajl_val *node, const char *name[]);	
void *get_json_array(yajl_val *node, const char *name[], size_t type_size);

#endif
