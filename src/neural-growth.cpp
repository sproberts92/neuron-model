#include <iostream>
#include <ctime>
#include <sstream>

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

	std::valarray<std::pair<double, double>> bounds = {{-50, 50}, {-50, 50}, {-50, 50}};

	Brain brain(config.schwann_l);
	brain.place_neurons(config.n_neurons, bounds);

	std::cout << "Neuron layout complete. Growing axons..." << std::endl;

	for (int i = 0; i < config.growth_iter; i++)
	{
		std::cout << 100 * i / config.growth_iter << "%\r";

		brain.grow_axons();
		brain.print_network(iter_filename(config.growth, i), 1);
	}
	std::cout << "100%\n" << std::endl;

	std::cout << "Axon growth complete. Growing dendrites..." << std::endl;

	brain.connect_network();

	std::ostringstream fileName;
	fileName << config.network.dir << "\\" << config.network.name << "." << config.network.ext;
	brain.print_network(fileName, 1);

	brain.find_loops();
	std::cout << std::endl;

	brain.clear_signals();
	brain.insert_signal(0);

	std::cout << "Writing signal propagation frames..." << std::endl;

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";

		brain.propagate_signal(0.0);
		brain.print_network(iter_filename(config.signal_prop, i), 0);

		// Kill the signal
		// if(brain.neurons.front().get_root()->get_value() == 1)
			// brain.neurons.front().get_root()->set_value(0);
	}

	std::cout << "100%\n" << std::endl;

	std::cout << "Network size: " << brain.network_size() << " nodes." << std::endl;
	// std::cout << "Connections: " << brain.all_synapses.size() << " synapses." << std::endl << std::endl;

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