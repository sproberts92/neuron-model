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

	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 1; ++j)
			simulate(i, j, config);

	return EXIT_SUCCESS;
}

void simulate(int i, int j, user_config_t &config)
{
	clock_t begin = clock();

	Brain *brain = new Brain;
	brain->create_network(config);

	std::cout << "Network size: " << brain->network_size() << " nodes." << std::endl;

	std::cout << "Network built in " << double(clock() - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;
	std::cout << "Running..." << std::endl;

	brain->path_nodes = brain->all_nodes;

	visualise(brain, config);
	// write_propagation_loop_frames(brain, config, i, j);

	delete brain;
}

void visualise(Brain *brain, user_config_t &config)
{
	glsr::Context *context = new glsr::Context;

	std::vector<double> particles;
	for(auto i : brain->all_nodes)
		for(auto &j : i->get_pos())
			particles.push_back(j);

	context->p_systems.push_back(glsr::Particles(10000000, "src\\shaders\\vertex_shader_p.glsl", "src\\shaders\\fragment_shader_p.glsl"));
	context->p_systems.push_back(glsr::Particles(100000, "src\\shaders\\vertex_shader_p2.glsl", "src\\shaders\\fragment_shader_p2.glsl"));

	brain->clear_signals();

	std::vector<double> particles2;
	particles2.reserve(10000);

	for (int i = 0; i < 10000; ++i)
		particles2.push_back(0.0f);

	int nn = config.n_neurons/2;
	// nn = 1;
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
			if (i->get_value())
				for(auto &j : i->get_pos())
					particles2[ii++] = j;

		context->p_systems[1].update_pp_data(particles2, ii);

		context->draw();

		if((glfwGetTime() - init) > 0.01)
		{
			std::cout << brain->propagate_signal(0.0)  << std::endl;
			init = glfwGetTime();
		}
	}

	glfwTerminate();

	delete context;
}

void write_propagation_loop_frames(Brain *brain, user_config_t &config, int i, int j)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain->clear_signals();

	for (int i = 0; i < config.n_neurons; ++i)
		brain->insert_signal(i);

	auto f  = file_name(config.activity,{config.n_neurons, (int)config.link_fwhm_param, i, j});
	std::ofstream out_stream(f.str(), std::fstream::trunc);

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";
		out_stream << brain->propagate_signal(0.0) << std::endl;
	}

	brain->clear_signals();

	std::cout << "100%\n" << std::endl;
}
