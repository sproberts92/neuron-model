#include "config.h"

void getConfigInfo(user_config_t *config)
{
	config_t cfg;
	config_setting_t *setting;
	config_init(&cfg);

	printf("Reading configuration settings... ");

	if(config_read_file(&cfg, "conf/config.cfg") != CONFIG_TRUE)
	{
		fprintf(stderr, "\nConfiguration read failed. Check conf/config.cfg "
			"exists and is correctly formatted.\n");
		exit(EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, "application.settings");

	int lookup_success = 1;
	lookup_success &= config_setting_lookup_int(setting, "dim", &(config->dim));
	lookup_success &= config_setting_lookup_int(setting, "n_neurons", &(config->n_neurons));
	lookup_success &= config_setting_lookup_int(setting, "growth_iter", &(config->growth_iter));
	lookup_success &= config_setting_lookup_int(setting, "prop_iter", &(config->prop_iter));
	lookup_success &= config_setting_lookup_int(setting, "target_age", &(config->target_age));
	lookup_success &= config_setting_lookup_int(setting, "combination_rule", &(config->combination_rule));
	lookup_success &= config_setting_lookup_float(setting, "schwann_l", &(config->schwann_l));
	lookup_success &= config_setting_lookup_float(setting, "link_fwhm_param", &(config->link_fwhm_param));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.settings.bounds");
	check_lookup(setting);
	int count = config_setting_length(setting);

	config->bounds = malloc(count * sizeof(double));

	for (int i = 0; i < count; ++i)
		config->bounds[i] = config_setting_get_float_elem(setting, i);

	setting = config_lookup(&cfg, "application.paths.growth");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->growth.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->growth.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->growth.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.network_c");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->network_c.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->network_c.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->network_c.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.network_n");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->network_n.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->network_n.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->network_n.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.network_r");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->network_r.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->network_r.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->network_r.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.signal_prop");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->signal_prop.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->signal_prop.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->signal_prop.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.neuron_adj");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->neuron_adj.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->neuron_adj.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->neuron_adj.ext));
	check_val_lookup(lookup_success);

	setting = config_lookup(&cfg, "application.paths.activity");
	check_lookup(setting);

	lookup_success = 1;
	lookup_success &= config_setting_lookup_string(setting, "dir", &(config->activity.dir));
	lookup_success &= config_setting_lookup_string(setting, "name", &(config->activity.name));
	lookup_success &= config_setting_lookup_string(setting, "ext", &(config->activity.ext));
	check_val_lookup(lookup_success);

	/* Would be nice to break the above out into a function to avoid repetition */

	printf("Success.\n");
}

void check_lookup(config_setting_t *setting)
{
	if(setting == NULL)
	{
		fprintf(stderr, "\nConfiguration lookup failed. Check format of conf/config.cfg.\n");
		exit(EXIT_FAILURE);
	}
}

void check_val_lookup(int lookup_success)
{
	if(!lookup_success)
	{
		fprintf(stderr, "\nConfiguration lookup failed. Check format of conf/config.cfg.\n");
		exit(EXIT_FAILURE);
	}
}
