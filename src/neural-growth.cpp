#include <iostream>
#include <ctime>
#include <sstream>

#include "brain.h"

extern "C" {
	#include "config.h"
}

int main()
{
	clock_t begin = clock();

	user_config_t config;
	getConfigInfo(&config);

	std::vector<double> bounds = {-50,50,-50,50,-50,50};
	
	Brain brain(config.dim, bounds, config.n_neurons, config.schwann_l);
	brain.place_neurons();

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int i = 0; i < config.growth_iter; i++)
	{
		std::cout << 100 * i / config.growth_iter << "%\r";

		brain.grow_axons();

		std::ostringstream fileName;
		fileName << "output\\growth\\growth_"<< i << ".dat";
		brain.print_network(fileName, 1);
	}
	std::cout << "100%\n" << std::endl;


	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;

	brain.connect_network();

	std::ostringstream fileName;
	fileName << "output\\Complete_Network.dat";
	brain.print_network(fileName, 1);

	std::vector<Node*> path;
	brain.depth_first_path_search(*brain.neurons[0].get_root(), *brain.neurons[0].get_root(), path);
	std::cout << std::endl;

	// Reset newtwork
	brain.clear_signals();
	brain.neurons[0].get_root()->set_value(1);

	std::cout << "Writing signal propagation frames..." << std::endl;

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";

		brain.propagate_signal(0.0);

		std::ostringstream fileName;
		fileName << "output\\signal\\signal_" << i << ".dat";
		brain.print_network(fileName, 0);

		// Kill the signal
		if(brain.neurons[0].get_root()->get_value() == 1)
			brain.neurons[0].get_root()->set_value(0);
	}

	std::cout << "100%\n" << std::endl;

	std::cout << "Network size: " << brain.network_size() << " nodes." << std::endl;
	// std::cout << "Connections: " << brain.all_synapses.size() << " synapses." << std::endl << std::endl;

	clock_t end = clock();
	std::cout << "Runtime: " << double(end - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}

