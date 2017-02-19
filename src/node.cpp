#include "node.hpp"

Node::Node(const std::valarray<double> p)
	: pos(p), value(0), on(true), buffer(0) {}

bool Node::get_value(void) { return value; } //
void Node::set_value(const bool v) { value = v; } //

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

bool Node::pop_temp(double noise, Statistics &s)
{
	auto r_gen = rand_gen<double>(0.0, 1.0);

	if(on && (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
		value = buffer > 0 ? 1 : 0;

	buffer = 0;

	return (bool)value;
}

void Node::update_threshold(void) {}

Neuron::Neuron(const std::valarray<double> p, int t) : Node(p), thresh(t), last_visited(0)
{
	history.resize(1<<9, 0);
}

void Neuron::gather_statistics(Statistics &s)
{
	s.pattern_found.clear();

	int len = 6;
	std::array<bool, 6> pat_win;

	for (int i = 0; i < len; ++i)
		pat_win[i] = history[i];

	for (int i = len; i < history.size() - len; ++i)
	{
		int success = 0;

		if (std::accumulate(pat_win.begin(), pat_win.end(), 0) >= 3)
		{
			for (int j = 0; j < len; ++j)
				if (history[i + j] == pat_win[j])
					success++;
				else
					break;

			if(success == len){
				s.pattern_found.push_back(i);
			}
		}
	}
}

bool Neuron::pop_temp(double noise, Statistics &s)
{
	auto r_gen = rand_gen<double>(0.0, 1.0);

	if(on && buffer >= thresh && (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
	{
		s.last_visited = last_visited;
		value = buffer;
		history[0] = true;
	}
	else
		history[0] = false;

	gather_statistics(s);

	if(buffer)
		last_visited = 0;

	buffer = 0;

	return (bool)value;
}

void Neuron::update_threshold(void)
{
	last_visited++;
	history.pop_back();
	history.push_front(false);
}

Synapse::Synapse(const std::valarray<double> p, int t) : Node(p), target_age(t), last_visited(-1.0f), window(20.0f) {}

bool Synapse::pop_temp(double noise, Statistics &s)
{
	auto r_gen = rand_gen<double>(0.0, 1.0);
	if(		on
			// && last_visited > target_age
			&& r_gen.get_rnum() > std::max((target_age - last_visited)/window, (double)0.0f)
			&& (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise))
		){
		value = buffer;
	}

	if(buffer)
		last_visited = 0;

	buffer = 0;

	return (bool)value;
}

void Synapse::update_threshold(void)
{
	last_visited++;
}
