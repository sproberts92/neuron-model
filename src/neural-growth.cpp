using namespace std;

#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "brain.h"

extern "C" {
	#include "config.h"
}

std::ostringstream iter_filename(path_t &path, int it);

int main()
{
	clock_t begin = clock();

	user_config_t config;
	getConfigInfo(&config);

	std::vector<double> bounds = {-50,50,-50,50,-50,50};
	
	Brain brain(config.dim, bounds, config.n_neurons, config.schwann_l);
	brain.place_neurons();

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int it = 0; it < config.growth_iter; it++)
	{
		if(it % (config.growth_iter / 100) == 0)
			std::cout << 100 * it / config.growth_iter << "%\r";

		brain.grow_axons();
		brain.print_network(iter_filename(config.growth, it), 1);
	}
	std::cout << "100%\n" << std::endl;

	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;

	brain.connect_network();

	brain.neurons[0].base_soma->value = 1;

	for (int it = 0; it < config.prop_iter; it++)
	{
		brain.propagate_signal();
		brain.print_network(iter_filename(config.signal_prop, it), 0);

		if(brain.neurons[0].base_soma->value == 1){
			std::cout << "Loop found! Length: " << it << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
			brain.neurons[0].base_soma->value = 0;
		}
	}

	std::cout << "Network size: " << brain.all_nodes.size() << " nodes." << std::endl;

	std::ostringstream fileName;
	fileName << config.network.dir << "\\" << config.network.name << "." << config.network.ext;
	brain.print_network(fileName, 1);

	clock_t end = clock();
	std::cout << "Runtime: " << double(end - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}

std::ostringstream iter_filename(path_t &path, int it)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name << it << "." << path.ext;
	return fileName;	
}