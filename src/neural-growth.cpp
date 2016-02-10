#pragma warning(disable : 1744)

#include <iostream>
#include <ctime>
#include <sstream>

#include <chrono>
#include <thread>

#include "glsr.hpp"

#include "brain.hpp"
#include "interface.hpp"
#include "messages.hpp"

void write_propagation_loop_frames(Brain &brain, user_config_t &config, int i);
bool compare_messages(std::vector<bool> message_1, std::vector<bool> message_2);

int main()
{
	clock_t begin = clock();

	user_config_t config;
	getConfigInfo(&config);

	Brain brain;
	brain.create_network(config);

	std::cout << "Printing network. ";
	brain.print_network(file_name(config.network_c), 0, 0);
	brain.print_neuron_adj(file_name(config.neuron_adj, {config.n_neurons, (int)config.link_fwhm_param}));
	std::cout << "Done." << std::endl;

	// std::cout << "Searching for loops..." << std::endl;
	// brain.find_loops(3);
	// brain.print_network(file_name(config.network_r), 1, 0);

	// write_propagation_loop_frames(brain, config);

	std::cout << "Network size: " << brain.network_size() << " nodes." << std::endl;
	std::cout << "Connections: " << brain.connections() << " synapses." << std::endl << std::endl;

	std::cout << "Network built in " << double(clock() - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;
	std::cout << "Running..." << std::endl;

	brain.path_nodes = brain.all_nodes;

	glsr::Context context;

	std::vector<double> particles;
	for(auto i : brain.all_nodes)
		for(auto &j : i->get_pos())
			particles.push_back(j);

	context.p_systems.push_back(glsr::Particles(1000000, "src\\shaders\\vertex_shader_p.glsl", "src\\shaders\\fragment_shader_p.glsl"));
	context.p_systems.push_back(glsr::Particles(1000, "src\\shaders\\vertex_shader_p2.glsl", "src\\shaders\\fragment_shader_p2.glsl"));

	brain.clear_signals();

	std::vector<double> particles2;
	particles2.reserve(10000);

	for (int i = 0; i < 10000; ++i)
		particles2.push_back(0.0f);

	int nn = config.n_neurons/4;
	std::cout << nn << std::endl;
	for (int i = 0; i < nn; ++i)
		brain.insert_signal(i);

	double init = glfwGetTime();
	while(!glfwWindowShouldClose(context.window))
	{
		context.p_systems[0].update_pp_data(particles, particles.size());

		int ii = 0;
		for(auto i : brain.all_nodes)
			if (i->get_value())
				for(auto &j : i->get_pos())
					particles2[ii++] = j;
		context.p_systems[1].update_pp_data(particles2, ii);

		context.draw();

		if((glfwGetTime() - init) > 0.05)
		{
			brain.propagate_signal(0.0);
			init = glfwGetTime();
		}
	}

	return EXIT_SUCCESS;
}

bool compare_messages(std::vector<bool> message_1, std::vector<bool> message_2)
{
	std::vector<bool> mask = {1, 1, 1, 1, 0, 0, 0, 1};

	if(message_1.size() && message_2.size())
	{
		bool match = true;
		for (int i = 0; i < mask.size(); ++i)
			if(mask[i] && message_1[i] != message_2[i])
				match = false;

		return match;
	}

	else return false;
}

void write_propagation_loop_frames(Brain &brain, user_config_t &config, int i)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain.clear_signals();
	// brain.insert_signal(0);

	for (int i = 0; i < config.n_neurons/4; ++i)
	{
		brain.insert_signal(i);
	}

	auto f  = file_name(config.activity,{config.n_neurons, (int)config.link_fwhm_param, i});
	std::ofstream out_stream(f.str(), std::fstream::trunc);

	for (int i = 0; i < config.prop_iter; i++)
	{
		// brain.insert_signal(0);
		// std::cout << 100 * i / config.prop_iter << "%\r";

		out_stream << brain.propagate_signal(0.0) << std::endl;
		// brain.print_network(file_name(config.signal_prop, i), 0, 1);
	}



	brain.clear_signals();

	std::cout << "100%\n" << std::endl;
}
