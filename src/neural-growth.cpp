using namespace std;

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
		if(i % (config.growth_iter / 100) == 0)
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

	brain.neurons[0].base_soma->value = 1;

	int first_target_loop = config.prop_iter;
	for (int i = 0; i < config.prop_iter; i++)
	{
		brain.propagate_signal(0);

		std::ostringstream fileName;
		fileName << "output\\signal\\signal_" << i << ".dat";
		brain.print_network(fileName, 0);

		if(	brain.neurons[0].base_soma->value == 1
			&& brain.neurons[0].base_soma->num_incoming >= 3
			&& i < first_target_loop)
		{
			std::cout << "Loop found! Length: " << i << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
			first_target_loop = i;
			brain.neurons[0].base_soma->value = 0;
			break;
		}
		else if(brain.neurons[0].base_soma->value == 1){
			std::cout << "Loop found! Length: " << i << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
			brain.neurons[0].base_soma->value = 0;
		}		
	}
	if(first_target_loop == config.prop_iter)
		std::cout << "No loops of target degeneracy found." << std::endl;
	else
		std::cout << "First loop of target degeneracy found with length " << first_target_loop << std::endl;


	/* Possibly remove this later for speed increase */
	first_target_loop = config.prop_iter;

	brain.clear_signals();
	brain.neurons[0].base_soma->value = 1;

	for (int a = 0; a < brain.all_synapses.size(); a++)
	{
		std::cout << "activations: " << a << std::endl;
		static rand_gen<int> r_gen(0, (int)brain.all_synapses.size());
		
		/* First turn off all synapses */
		for(auto it_s = brain.all_synapses.begin(); it_s != brain.all_synapses.end(); ++it_s)
			(*it_s)->on = 0;
		
		int activations = 0;
		while(activations < a)
		{
			int s_ind = r_gen.get_rnum();
			if(!brain.all_synapses[s_ind]->on)
			{
				brain.all_synapses[s_ind]->on = 1;
				activations++;
			}	
		}

		for (int i = 0; i < first_target_loop; i++)
		{
			brain.propagate_signal(0);

			if(	brain.neurons[0].base_soma->value == 1
				&& brain.neurons[0].base_soma->num_incoming >= 3
				&& i < first_target_loop)
			{
				std::cout << "Loop found! Length: " << i << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
				first_target_loop = i;
				brain.neurons[0].base_soma->value = 0;
				break;
			}
			else if(brain.neurons[0].base_soma->value == 1){
				std::cout << "Loop found! Length: " << i << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
				brain.neurons[0].base_soma->value = 0;
			}
		}
	}


	std::cout << "Network size: " << brain.all_nodes.size() << " nodes." << std::endl;
	std::cout << "Connections: " << brain.all_synapses.size() << " synapses." << std::endl << std::endl;

	clock_t end = clock();
	std::cout << "Runtime: " << double(end - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}

