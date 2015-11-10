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

	/************ Naive pruning algorithm ************/
	/* Possibly remove this later for speed increase */
	std::cout << first_target_loop << std::endl;
	// first_target_loop = config.prop_iter;
	first_target_loop += 5;


	/* a: attempt number */
	bool found = false;
	for (int prune = 0; prune < 20; prune++)
	{
		
	for (int a = 0; a < brain.all_synapses.size(); a++)
	{
		static rand_gen<int> r_gen(0, (int)brain.all_synapses.size());

		std::cout << "a = " << a << std::endl;
		
		/* Reset newtwork */
		brain.clear_signals();
		brain.neurons[0].base_soma->value = 1;

		/* Reset synapses */
		for(auto it_s = brain.synapses.begin(); it_s != brain.synapses.end(); ++it_s)
			it_s->second = 0;

		found = false;
		for (int i = 0; i < first_target_loop; i++)
		{
			brain.check_path(0.0);

			if(	brain.neurons[0].base_soma->value == 1
				&& brain.neurons[0].base_soma->num_incoming >= 3)
			{
				std::cout << "Loop found! Length: " << i << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
				brain.neurons[0].base_soma->value = 0;
				found = true;
				break;
			}
			else if(brain.neurons[0].base_soma->value == 1){
				brain.neurons[0].base_soma->value = 0;
			}
		}

		if(found) break;
	}
	}

	if(!found)
	{
		std::cout << "Did not find loop!" << std::endl;
		exit(EXIT_FAILURE);
	}

	for(auto it_s = brain.synapses.begin(); it_s != brain.synapses.end(); ++it_s)
	{
		if(it_s->second == 1)
			it_s->first->on = true;
		else if(it_s->second == -1)
			it_s->first->on = false;
		else
			std::cout << "Synapse error!" << std::endl;
	}

	/* Reset newtwork */
	brain.clear_signals();
	brain.neurons[0].base_soma->value = 1;
	/*********End naive pruning algorithm ************/

	for (int i = 0; i < config.prop_iter; i++)
	{
		brain.propagate_signal(0.0);

		std::ostringstream fileName;
		fileName << "output\\signal\\signal_" << i << ".dat";
		brain.print_network(fileName, 0);	
	}


	std::cout << "Network size: " << brain.all_nodes.size() << " nodes." << std::endl;
	std::cout << "Connections: " << brain.all_synapses.size() << " synapses." << std::endl << std::endl;

	clock_t end = clock();
	std::cout << "Runtime: " << double(end - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}

