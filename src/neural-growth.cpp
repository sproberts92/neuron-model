using namespace std;

#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>

#include "neuron.h"

int main()
{
	clock_t begin = clock();

	int dim = 3;
	int n_neurons = 5;
	int growth_iter = 200;
	std::vector<double> bounds = {-50,50,-50,50,-50,50};
	
	// Not currently used, future: used to create variation in Schwann cell length
	// std::vector<double> s_bounds = {7.7, 7.8};
	
	neuron *neurons = new neuron[n_neurons];
	

	for (int i = 0; i < n_neurons; i++)
		neurons[i] = neuron(dim, bounds);
		
	for (int it = 0; it <= growth_iter; it++)
	{
		std::ostringstream fileName;
		fileName << "output\\output_"<< it << ".dat";
		
		for (int i = 0; i < n_neurons; i++)
				neurons[i].grow_axon(1.0, bounds);
		
		ofstream outfile;
		outfile.open(fileName.str());
		for (int i = 0; i < n_neurons; i++)
			neurons[i].print_axon(outfile);
		outfile.close();
	}	

	clock_t end = clock();
	std::cout << double(end - begin) /CLOCKS_PER_SEC << std::endl;

	return 0;
}