using namespace std;

#include <iostream>
#include <ctime>
#include <fstream>
#include <sstream>

#include "brain.h"

void print_axon(int dim, node *base, std::ofstream &out_stream);

int main()
{
	clock_t begin = clock();

	int dim = 3;
	int n_neurons = 15;
	int growth_iter = 100;
	double schwann_l = 1.0;
	std::vector<double> bounds = {-50,50,-50,50,-50,50};
		
	neuron *neurons = new neuron[n_neurons];
	
	brain brain_1(dim, bounds, n_neurons, schwann_l);
	brain_1.place_neurons();

	for (int it = 0; it < growth_iter; it++)
	{
		std::ostringstream fileName;
		fileName << "output\\output_"<< it << ".dat";

		brain_1.grow_axons();

		ofstream outfile;
		outfile.open(fileName.str(), 'w');
		for (int i = 0; i < n_neurons; i++)
			print_axon(dim, brain_1.neurons[i].base_soma, outfile);
		outfile.close();
	}

	brain_1.connect_network();

	ofstream outfile;
	outfile.open("output\\finalprint.dat", std::ofstream::trunc);
		for (int i = 0; i < n_neurons; i++)
			print_axon(dim, brain_1.neurons[i].base_soma, outfile);
	outfile.close();

	clock_t end = clock();
	std::cout << double(end - begin) /CLOCKS_PER_SEC << std::endl;

	return 0;
}

void print_axon(int dim, node *base, std::ofstream &out_stream)
{
	node *list_ptr;
	list_ptr = base;

	if(list_ptr != 0)
		while(!list_ptr->next.empty())
		{
			for (int i = 0; i < dim; i++)
				out_stream << list_ptr->pos[i] << " ";
			out_stream << std::endl;

			for (int i = 1; i < list_ptr->next.size(); i++)
				print_axon(dim, list_ptr->next[i], out_stream);

			list_ptr = list_ptr->next[0];
		}
}