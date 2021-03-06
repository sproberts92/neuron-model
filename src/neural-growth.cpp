#include <iostream>
#include <ctime>
#include <sstream>

#include <chrono>
#include <thread>

#include "brain.hpp"
#include "interface.hpp"
#include "messages.hpp"

#pragma warning( push, 0 )
#include "glsr.hpp"
#pragma warning( pop )

// Place signals at neurons and propagate the signals through the network while
// gathering statistics and writing output data to file, including network
// activity levels and recurrence of patterns of activity at neurons.
void write_propagation_loop_frames(Brain *brain, user_config_t &config, int i, int j);

// Place one signal at each neuron and propagate the signals through the
// network with OpenGL visualisation, the simulation will run indefinitely
// until cancelled by the user with ESC. The program will exit at then end of
// this function so that this function can be used without modifying main() to
// remove loops.
// To do: Read the proportion of neurons where a signal is placed from config.
void visualise(Brain *brain, user_config_t &config);

// Run a single simulation, consisting of growing and connecting a network,
// placing signals in the network, allowing propagation of the signals through
// the network and gathering statistics on this propagation OR growing a single
// network, placing signals and allowing propagation with OpenGL visualisation.
// In this case the program exits after a single simulation.
// To do: At the moment the functionality is hard selected but should be read
// from config.
void simulate(int i, int j, user_config_t &config);

int main()
{
	user_config_t config;
	getConfigInfo(&config);

	for (int ii = 0; ii < 500; ii += 10)
		for (int jj = 0; jj < 10; ++jj)
			simulate(ii, jj, config);

	return EXIT_SUCCESS;
}

void simulate(int ii, int jj, user_config_t &config)
{
	// config.link_fwhm_param = ii/10.0f;

	clock_t begin = clock();

	Brain *brain = new Brain;
	brain->create_network(config);

	std::cout << "Network size: " << brain->network_size() << " nodes." << std::endl;

	std::cout << "Network built in " << double(clock() - begin) /CLOCKS_PER_SEC
	<< " seconds." << std::endl;
	std::cout << "Running..." << std::endl;

	// To do: Toggle the below functionality from config.
	//
	// brain->path_nodes = brain->all_nodes;
	//
	// auto fl  = file_name("output/three_loops/tl", {config.n_neurons, (int)config.link_fwhm_param, ii, jj});
	// std::ofstream out_stream(fl.str(), std::fstream::app);
	// out_stream << brain->find_loops(3) << std::endl;
	//
	// brain->print_network(file_name(config.network_c, 1), 1, 0);
	//
	// auto f  = file_name(config.neuron_adj,{config.n_neurons, ii, jj});
	// brain->print_neuron_adj(f);
	//
	// visualise(brain, config);

	write_propagation_loop_frames(brain, config, ii, jj);

	delete brain;
}

void visualise(Brain *brain, user_config_t &config)
{
	glsr::Context *context = new glsr::Context;

	std::vector<double> particles;
	for(auto i : brain->all_nodes)
		for(auto &j : i->get_pos())
			particles.push_back(j);

	int max_particles2 = 100000;

	context->p_systems.push_back(glsr::Particles(10000000,
		"src/shaders/vertex_shader_p.glsl", "src/shaders/fragment_shader_p.glsl"));
	context->p_systems.push_back(glsr::Particles(max_particles2,
		"src/shaders/vertex_shader_p2.glsl", "src/shaders/fragment_shader_p2.glsl"));

	brain->clear_signals();

	std::vector<double> particles2;
	particles2.reserve(max_particles2);

	for (int i = 0; i < max_particles2; ++i)
		particles2.push_back(0.0f);

	int nn = config.n_neurons;
	std::cout << nn << std::endl;
	for (int i = 0; i < nn; ++i)
		brain->insert_signal(i);

	double init = glfwGetTime();
	int f = 0;
	while(!glfwWindowShouldClose(context->window))
	{
		context->p_systems[0].update_pp_data(particles, (int)particles.size());

		int ii = 0;
		for(auto i : brain->all_nodes)
		{
			if (i->get_value() && ii < max_particles2)
				for(auto &j : i->get_pos())
					particles2[ii++] = j;
		}

		context->p_systems[1].update_pp_data(particles2, ii);

		context->draw();

		Long_Run_Statistics lrs;

		if((glfwGetTime() - init) > 0.01)
		{
			brain->propagate_signal(0.001, lrs);
			init = glfwGetTime();
		}
	}

	glfwTerminate();

	delete context;

	exit(EXIT_SUCCESS);
}

void write_propagation_loop_frames(Brain *brain, user_config_t &config, int ii, int jj)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain->clear_signals();

	int nn = config.n_neurons;
	for (int i = 0; i < nn; ++i)
		brain->insert_signal(i);

	auto f  = file_name(config.activity,{config.n_neurons, (int)config.link_fwhm_param, ii, jj});
	std::ofstream out_stream(f.str(), std::fstream::trunc);

	Long_Run_Statistics lrs;

	for (int i = 0; i < config.prop_iter; i++)
	{
		brain->print_network(file_name(config.signal_prop, i), 0, 1);

		std::cout << 100 * i / config.prop_iter << "%\r";
		out_stream << brain->propagate_signal(0.001, lrs) << std::endl;
	}

	for(auto it : lrs.last_visited)
		std::cout << it << " ";
	std::cout << std::endl;

	std::ofstream out_stream2("output/PatternFound.dat", std::fstream::app);

	for(auto it : lrs.pattern_found)
		out_stream2 << it << " ";
	out_stream2 << std::endl;

	brain->clear_signals();

	std::cout << "100%\n" << std::endl;
}
