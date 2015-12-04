#include <iostream>
#include <queue>
#include <ctime>

#include "brain.h"
#include "interface.h"

void write_propagation_loop_frames(Brain &brain, user_config_t &config);

int main()
{
	clock_t begin = clock();

	user_config_t config;
	getConfigInfo(&config);

	Brain brain;
	brain.create_network(config);

	std::cout << "Printing network. ";
	brain.print_network(file_name(config.network), 0, 0);
	brain.print_neuron_adj(file_name(config.neuron_adj, {config.n_neurons, (int)config.link_fwhm_param}));
	std::cout << "Done." << std::endl;

	std::cout << "Searching for loops..." << std::endl;
	brain.find_loops(3);
	brain.print_network(file_name(config.network), 1, 0);

	// write_propagation_loop_frames(brain, config);

	std::cout << "Network size: " << brain.network_size() << " nodes." << std::endl;
	std::cout << "Connections: " << brain.connections() << " synapses." << std::endl << std::endl;

	std::cout << "Runtime: " << double(clock() - begin) /CLOCKS_PER_SEC << " seconds." << std::endl;

	key_state_t key_state;

	std::queue<bool> queue;
	
	int i = 0;
	while(!key_state.esc)
	{
		read_key_state(key_state);
		
		std::vector<bool> message;
		
		if(key_state.a && key_state.g)
			message = {1,0,1,0,0,1,1,1};
		else if(key_state.a)
			message = {1,0,1,0,0,0,0,1};
		
		std::vector<bool> message_r;
		brain.read_message(message_r);

		for(auto s : message_r)
			std::cout << s;

		std::cout << " ";
		if(message.size())
			for(auto s : message)
				std::cout << s;

		if(		message.size()	
			&&	message[0] == message_r[0]
			&&	message[1] == message_r[1]
			&&	message[2] == message_r[2]
			&&	message[7] == message_r[7])	std::cout << "A is good!" << std::endl;
		std::cout << std::endl;

		if(!queue.size() && message.size() && message[6])
			for(auto it : message)
				queue.push(it);

		message.clear();

		if(queue.size())
		{
			if(queue.front()) brain.insert_signal(0);
			queue.pop();
		}

		brain.print_network(file_name(config.signal_prop, i++), 1, 1);
		brain.propagate_signal(0.0);
	}

	return 0;
}

void write_propagation_loop_frames(Brain &brain, user_config_t &config)
{
	std::cout << "Writing signal propagation frames..." << std::endl;

	brain.clear_signals();
	brain.insert_signal(0);

	for (int i = 0; i < config.prop_iter; i++)
	{
		std::cout << 100 * i / config.prop_iter << "%\r";

		brain.propagate_signal(0.0);
		brain.print_network(file_name(config.signal_prop, i), 1, 1);
	}

	brain.clear_signals();

	std::cout << "100%\n" << std::endl;
}