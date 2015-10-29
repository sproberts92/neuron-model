using namespace std;

#include <iostream>
#include <ctime>
#include <sstream>

#include "brain.h"

int main()
{
	clock_t begin = clock();

	int dim = 3;
	int n_neurons = 8;
	int growth_iter = 100;
	int prop_iter = 500;
	double schwann_l = 1.0;
	std::vector<double> bounds = {-50,50,-50,50,-50,50};
	
	Brain brain(dim, bounds, n_neurons, schwann_l);
	brain.place_neurons();

	for (int it = 0; it < growth_iter; it++)
	{
		brain.grow_axons();

		std::ostringstream fileName;
		fileName << "output\\growth\\growth_"<< it << ".dat";
		brain.print_network(fileName, 1);
	}

	brain.connect_network();

	brain.neurons[0].base_soma->value = 1;

	for (int it = 0; it < prop_iter; it++)
	{
		brain.propagate_signal();

		std::ostringstream fileName;
		fileName << "output\\signal\\signal_"<< it << ".dat";
		brain.print_network(fileName, 0);

		if(brain.neurons[0].base_soma->value == 1){
			std::cout << "Loop found! Length: " << it << ", degeneracy: " <<brain.neurons[0].base_soma->num_incoming<< std::endl;
			brain.neurons[0].base_soma->value = 0;
		}
	}

	std::cout << "Network size: " << brain.all_nodes.size() << " nodes." << std::endl;

	std::ostringstream fileName;
	fileName << "output\\Complete_Network.dat";
	brain.print_network(fileName, 1);

	clock_t end = clock();
	std::cout << "Runtime: " << double(end - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	return 0;
}

