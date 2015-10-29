using namespace std;

#include <iostream>
#include <ctime>
#include <sstream>

#include "brain.h"

int main()
{
	clock_t begin = clock();

	int dim = 3;
	int n_neurons = 15;
	int growth_iter = 100;
	double schwann_l = 1.0;
	std::vector<double> bounds = {-50,50,-50,50,-50,50};
	
	Brain brain(dim, bounds, n_neurons, schwann_l);
	brain.place_neurons();

	for (int it = 0; it < growth_iter; it++)
	{
		brain.grow_axons();

		std::ostringstream fileName;
		fileName << "output\\output_"<< it << ".dat";
		brain.print_network(fileName);
	}

	brain.connect_network();

	std::cout << brain.all_nodes.size() << std::endl;

	std::ostringstream fileName;
	fileName << "output\\finalprint.dat";
	brain.print_network(fileName);

	clock_t end = clock();
	std::cout << double(end - begin) /CLOCKS_PER_SEC << std::endl;

	return 0;
}

