#include <iostream>
#include <ctime>

#include "brain.h"
#include "interface.h"

int main()
{
	clock_t begin = clock();

	user_config_t config;
	getConfigInfo(&config);

	Brain brain;
	brain.create_network(config);

	brain.print_network(file_name(config.network), 1);

	std::cout << "Searching for loops..." << std::endl;
	brain.find_loops(3);

	brain.clear_signals();
	brain.insert_signal(0);

	std::cout << "Writing signal propagation frames..." << std::endl;

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";

		brain.propagate_signal(0.0);
		brain.print_network(file_name(config.signal_prop, i), 0);

		// Kill the signal
		// if(brain.read_signal(0) == 1)
			// brain.neurons.front().get_root()->set_value(0);
	}

	std::cout << "100%\n" << std::endl;

	std::cout << "Network size: " << brain.network_size() << " nodes." << std::endl;
	std::cout << "Connections: " << brain.connections() << " synapses." << std::endl << std::endl;

	std::cout << "Runtime: " << double(clock() - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}
