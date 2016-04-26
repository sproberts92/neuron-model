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

void write_propagation_loop_frames(Brain *brain, user_config_t &config, int i, int j);
bool compare_messages(std::vector<bool> message_1, std::vector<bool> message_2);
void visualise(Brain *brain, user_config_t &config);
void simulate(int i, int j, user_config_t &config);

int main()
{
	user_config_t config;
	getConfigInfo(&config);

	for (int ii = 500; ii <= 500; ii += 25)
		for (int jj = 0; jj < 20; ++jj)
			simulate(ii, jj, config);

	return EXIT_SUCCESS;
}

void simulate(int ii, int jj, user_config_t &config)
{
	// config.n_neurons = ii;
	clock_t begin = clock();

	Brain *brain = new Brain;
	brain->create_network(config);

	std::cout << "Network size: " << brain->network_size() << " nodes." << std::endl;

	std::cout << "Network built in " << double(clock() - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;
	std::cout << "Running..." << std::endl;

	brain->path_nodes = brain->all_nodes;

	visualise(brain, config);
	// write_propagation_loop_frames(brain, config, ii, jj);

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

	context->p_systems.push_back(glsr::Particles(10000000, "src\\shaders\\vertex_shader_p.glsl", "src\\shaders\\fragment_shader_p.glsl"));
	context->p_systems.push_back(glsr::Particles(max_particles2, "src\\shaders\\vertex_shader_p2.glsl", "src\\shaders\\fragment_shader_p2.glsl"));

	brain->clear_signals();

	std::vector<double> particles2;
	particles2.reserve(max_particles2);

	for (int i = 0; i < max_particles2; ++i)
		particles2.push_back(0.0f);

	int nn = config.n_neurons;
	// nn = 85;
	std::cout << nn << std::endl;
	for (int i = 0; i < nn; ++i)
		brain->insert_signal(i);

	double init = glfwGetTime();
	int f = 0;
	while(!glfwWindowShouldClose(context->window))
	{
		context->p_systems[0].update_pp_data(particles, particles.size());

		int ii = 0;
		for(auto i : brain->all_nodes)
		{
			if (i->get_value() && ii < max_particles2)
				for(auto &j : i->get_pos())
					particles2[ii++] = j;
		}

		context->p_systems[1].update_pp_data(particles2, ii);

		context->draw();

		if((glfwGetTime() - init) > 0.01)
		{
			brain->propagate_signal(0.001);
			init = glfwGetTime();
		}
	}

	glfwTerminate();

	delete context;
}

void write_propagation_loop_frames(Brain *brain, user_config_t &config, int ii, int jj)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain->clear_signals();

	for (int i = 0; i < 85; ++i)
		brain->insert_signal(i);

	auto f  = file_name(config.activity,{config.n_neurons, (int)config.link_fwhm_param, ii, jj});
	std::ofstream out_stream(f.str(), std::fstream::trunc);

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";
		out_stream << brain->propagate_signal(0.0) << std::endl;
	}

	brain->clear_signals();

	std::cout << "100%\n" << std::endl;
}
