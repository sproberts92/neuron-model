#include "config.h"

int read_config(user_config_t *cf)
{
	char errbuf[1024];
	yajl_val node;

	long length;
	char *file_data = read_file(&length, "conf/config.json");

	node = yajl_tree_parse((const char *) file_data, errbuf, sizeof(errbuf));

	if(check_parse_error(node, errbuf, 1)) return 0;

	const char * settings_dim[]            = {"settings", "dim"};
	cf->dim                                = get_json_int(&node, settings_dim);

	const char *settings_n_neurons[]       = {"settings", "n_neurons"};
	cf->n_neurons                          = get_json_int(&node , settings_n_neurons);

	const char *settings_growth_iter[]     = {"settings", "growth_iter"};
	cf->growth_iter                        = get_json_int(&node , settings_growth_iter);

	const char *settings_prop_iter[]       = {"settings", "prop_iter"};
	cf->prop_iter                          = get_json_int(&node , settings_prop_iter);

	const char *settings_schwann_l[]       = {"settings", "schwann_l"};
	cf->schwann_l                          = get_json_int(&node , settings_schwann_l);

	const char *settings_link_fwhm_param[] = {"settings", "link_fwhm_param"};
	cf->link_fwhm_param                    = get_json_int(&node , settings_link_fwhm_param);

/*	const char *settings_bounds[]          = {"settings", "bounds"};
	cf->bounds                             = get_json_array(&node , settings_bounds, sizeof(double));*/

	yajl_tree_free(node);
	free(file_data);

	return 1;
}

int get_json_int(yajl_val *node, char *name[])
{
	const char *path[] = {name[0], name[1], (const char *) 0 };
	yajl_val v = yajl_tree_get(*node, path, yajl_t_number);

	if(v)
		return (int)YAJL_GET_INTEGER(v);
	else
	{
		fprintf(stderr, "Can't find node %s/%s\n", name[0], name[1]);
		return -1;
	}
}

double get_json_double(yajl_val *node, const char *name[])
{
	const char *path[] = {name[0], name[1], (const char *) 0 };
	yajl_val v = yajl_tree_get(*node, path, yajl_t_number);

	if(v)
		return YAJL_GET_DOUBLE(v);
	else
	{
		fprintf(stderr, "Can't find node %s/%s\n", name[0], name[1]);
		return -1.0f;
	}
}

/* Waiting on S.E. answer as to why array is not properly parsed
void *get_json_array(yajl_val *node, const char *name[], size_t type_size)
{
	const char *path[] = {name[0], name[1], (const char *) 0 };
	yajl_val v = yajl_tree_get(*node, path, yajl_t_number);

	void *array = NULL;

	if(v){
		size_t len = YAJL_GET_ARRAY(v)->len;
		yajl_val *values = YAJL_GET_ARRAY(v)->values;

		array = malloc(len * type_size);

		for (int i = 0; i < len; ++i)
			memcpy((char *)array + (i * type_size), &values[i], type_size);

		free(values);
		return array;
	}
	else
	{
		fprintf(stderr, "Can't find node %s/%s\n", name[0], name[1]);
		return NULL;
	}
}
*/

int check_parse_error(yajl_val node, char *errbuf, int v)
{
	/* 0 for success, 1 for failure.
	 * makes a call like
	 *     if(check_parse_error(...)){ handle error }
	 * more natural.
	 * v argument is verbose, should error be printed to stderr */

	if (node == NULL)
	{
		if (v)
		{
			fprintf(stderr, "Parse error: ");

			if (strlen(errbuf))
				fprintf(stderr, " %s", errbuf);
			else
				fprintf(stderr, "unknown error.");

			fprintf(stderr, "\n");
		}

		return 1;
	}
	else
		return 0;
}

char *read_file(long int *length, const char *file_name)
{
	char *buffer = NULL;

	FILE *fp = fopen(file_name, "rb");
	if(!fp)
	{
		fprintf(stderr, "Could not open %s\n", file_name);
		*length = 0;
	}
	else
	{
		fseek(fp, 0, SEEK_END);
		long int string_size = ftell(fp);
		rewind(fp);

		buffer = malloc (sizeof(char) * (string_size + 1));
		long int read_size = fread(buffer, sizeof(char), string_size, fp);

		buffer[string_size] = '\0';

		*length = read_size;

		if(string_size != read_size)
		{
			fprintf(stderr, "Read failed.\n");
			free(buffer);
			buffer = NULL;
			*length = 0;
		}
	}

	fclose(fp);

	return(buffer);
}
