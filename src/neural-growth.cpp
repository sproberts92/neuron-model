#include <iostream>
#include <queue>
#include <ctime>

#include <chrono>
#include <thread>

#include "brain.hpp"
#include "interface.hpp"
#include "messages.hpp"

void write_propagation_loop_frames(Brain &brain, user_config_t &config);
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

	write_propagation_loop_frames(brain, config);

	return EXIT_SUCCESS;

/*	key_state_t key_state;

	std::queue<bool> queue;
	
	int i = 0;
	bool sent_flag = false;
	bool pressed_flag = false;
	while(!key_state.esc)
	{
		read_key_state(key_state);

		std::vector<bool> message;
		
		if(key_state.a && key_state.g)
			message = messages["ag"];
		else if(key_state.s && key_state.g)
			message = messages["sg"];
		else if(key_state.d && key_state.g)
			message = messages["dg"];
		else if(key_state.a && key_state.b)
			message = messages["ab"];
		else if(key_state.s && key_state.b)
			message = messages["sb"];
		else if(key_state.d && key_state.b)
			message = messages["db"];
		else if(key_state.a)
			message = messages["a"];
		else if(key_state.s)
			message = messages["s"];
		else if(key_state.d)
			message = messages["d"];
		
		std::vector<bool> message_r;
		brain.read_message(message_r);

		if(!key_state.key) std::cout << "\r          \r";

		if(compare_messages(message, message_r))
			if(key_state.a && !message[6] && message_r[5]) std::cout << "A is good!\r";
			else if(key_state.s && !message[6] && message_r[5]) std::cout << "S is good!\r";
			else if(key_state.d && !message[6] && message_r[5]) std::cout << "D is good!\r";
			else if(key_state.a && !message[6] && message_r[4]) std::cout << "A is bad!\r";
			else if(key_state.s && !message[6] && message_r[4]) std::cout << "S is bad!\r";
			else if(key_state.d && !message[6] && message_r[4]) std::cout << "D is bad!\r";

		if(pressed_flag && sent_flag && key_state.key) {}
		else if(key_state.key)
		{
			pressed_flag = true;
			if(queue.empty() && message.size() && message[6])
			{
				sent_flag = true;
				for(auto it : message)
					queue.push(it);
			}
			else sent_flag = false;
		}
		else pressed_flag = false;
		
		message.clear();

		if(queue.size())
		{
			if(queue.front()) brain.insert_signal(0);
			queue.pop();
		}

		// brain.print_network(file_name(config.signal_prop, i++), 1, 1);
		brain.propagate_signal(0.0);
	}*/

	return 0;
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

void write_propagation_loop_frames(Brain &brain, user_config_t &config)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain.clear_signals();
	// brain.insert_signal(0);

	for (int i = 0; i < config.n_neurons/4; ++i)
	{
		brain.insert_signal(i);
	}

	std::ofstream out_stream("output\\Activity.dat", std::fstream::trunc);

	for (int i = 0; i < config.prop_iter; i++)
	{
	// brain.insert_signal(0);
		// std::cout << 100 * i / config.prop_iter << "%\r";

		out_stream << brain.propagate_signal(0.0) << std::endl;
		brain.print_network(file_name(config.signal_prop, i), 0, 1);
	}



	brain.clear_signals();

	std::cout << "100%\n" << std::endl;
}