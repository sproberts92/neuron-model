#include "config.h"

void getConfigInfo(user_config_t *config)
{
	config_t cfg;
	config_setting_t *setting;
	config_init(&cfg);

	printf("Reading configuration settings... ");

	if(config_read_file(&cfg, "conf/config.cfg") != CONFIG_TRUE)
	{
		fprintf(stderr, "\nConfiguration read failed. Check conf/config.cfg exists.\n");
		exit(EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, "application.settings");
	if(setting == NULL)
	{
		fprintf(stderr, "\nConfiguration lookup failed. Check format of conf/config.cfg.\n");
		exit(EXIT_FAILURE);
	}

	if(
		   config_setting_lookup_int(setting, "dim", &(config->dim))
		&& config_setting_lookup_int(setting, "n_neurons", &(config->n_neurons))
		&& config_setting_lookup_int(setting, "growth_iter", &(config->growth_iter))
		&& config_setting_lookup_int(setting, "prop_iter", &(config->prop_iter))
		&& config_setting_lookup_float(setting, "schwann_l", &(config->schwann_l))
	)
	{
		printf("Success.\n");
	}
	else
	{
		fprintf(stderr, "\nConfiguration lookup failed. Check format of conf/config.cfg.\n");
		exit(EXIT_FAILURE);
	}

	// TODO: Add support for bounds
	// setting = config_lookup(&cfg, "application.settings.bounds");
	// int count = config_setting_length(setting);
	// printf("Count: %d\n", count);
}