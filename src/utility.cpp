#include "utility.hpp"

std::ostringstream file_name(path_t &path)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name << "." << path.ext;
	return fileName;
}

std::ostringstream file_name(path_t &path, int it)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name << it << "." << path.ext;
	return fileName;
}

std::ostringstream file_name(path_t &path, std::initializer_list<int> list)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name;

	for(auto i : list)
		fileName << "_" << i;

	fileName << "." << path.ext;
	return fileName;
}

std::ostringstream file_name(const std::string& path, std::initializer_list<int> list)
{
	std::ostringstream fileName;
	fileName << path;

	for(auto i : list)
		fileName << "_" << i;

	fileName << ".dat";
	return fileName;
}

std::valarray<std::pair<double, double>> convert_bounds(user_config_t &config)
{
	std::valarray<std::pair<double, double>> bounds(config.dim);

	for (int i = 0; i < config.dim; ++i)
	{
		bounds[i].first  = config.bounds[2 * i];
		bounds[i].second = config.bounds[2 * i + 1];
	}

	return bounds;
}
