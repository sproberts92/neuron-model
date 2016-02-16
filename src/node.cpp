#include "node.hpp"

Node::Node(const std::valarray<double> p)
	: pos(p), value(0), on(true), buffer(0) {}

bool Node::get_value(void) { return value; }
void Node::set_value(const bool v) { value = v; }

void Node::turn_on(void) { on = true; }
void Node::turn_off(void) { on = false; }

std::valarray<double> Node::get_pos(void) { return pos; }

std::vector<Node*> Node::get_next(void) { return next; }
void Node::push_next(Node &n) { next.push_back(&n); }

void Node::clear_signal(void)
{
	value = 0;
	buffer = 0;
}

void Node::push_temp_next(void)
{
	for(auto &it_next : next)
		it_next->buffer = it_next->buffer + value;

	value = 0;
}

bool Node::pop_temp(double noise)
{
	auto r_gen = rand_gen<double>(0.0, 1.0);

	// if(on && (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
		value = buffer > 0 ? 1 : 0;

	buffer = 0;

	return (bool)value;
}

Neuron::Neuron(const std::valarray<double> p, int t) : Node(p), thresh(t) {}

// bool Neuron::pop_temp(double noise)
// {
// 	auto r_gen = rand_gen<double>(0.0, 1.0);

// 	if(on && buffer >= thresh && (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
// 		value = buffer;

// 	buffer = 0;

// 	return (bool)value;
// }

Synapse::Synapse(const std::valarray<double> p) : Node(p), thresh(1.0f) {}

// bool Synapse::pop_temp(double noise)
// {
// 	auto r_gen = rand_gen<double>(0.0, 1.0);

// 	if(on && /*r_gen.get_rnum() <= thresh &&*/ (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
// 	{
// 		value = buffer;
// 		if(thresh <= 1.0f)
// 		{
// 			// thresh += 0.1;
// 		}
// 	}
// 	std::cout << "ThresH " << thresh << "a" << std::endl;

// 	// thresh -= 0.05;
// 	// std::cout << thresh << std::endl;
// 	buffer = 0;

// 	return (bool)value;
// }
