#include "node.hpp"

Node::Node(const std::valarray<double> p)
	: pos(p), value(0), on(true), buffer(0) {}

bool Node::get_value(void) { return !!value; }
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

	// This isn't usually needed, but is here more for safety, in case pop_temp
	// is changed such that the value isn't overridden and instead is updated.
	value = 0;
}

bool Node::pop_temp(double noise, Statistics &s)
{
	// Could this be made into a static class member?
	static auto r_gen = rand_gen<double>(0.0, 1.0);

	// Propagate if node is on and if has not been prevented by noise.
	if(on && (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
		value = buffer > 0 ? 1 : 0;

	// Buffer is cleared in advance for the next propagation step.
	buffer = 0;

	// !! explicitly converts to bool.
	return !!value;
}

void Node::update_threshold(void) {}

Neuron::Neuron(const std::valarray<double> p, int t)
	: Node(p), thresh(t), last_visited(0)
{
	// To do: read the history length from config.
	history.resize(1<<9, 0);
}

void Neuron::gather_statistics(Statistics &s)
{
	s.pattern_found.clear();

	// To do: read this from config.
	int len = 6;

	for (int i = len; i < history.size() - len; ++i)
	{
		int success = 0;

		// Look for the last len elements of the history at all points further
		// back in the history.
		if (std::accumulate(history.begin(), history.begin() + len, 0) >= 3)
		{
			for (int j = 0; j < len; ++j)
				if (history[i + j] == history[j])
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
	static auto r_gen = rand_gen<double>(0.0, 1.0);

	// On propagation, do the usual updates but also update the history and
	// last visited statistic (which is harder to update from inside the
	// gather_statistics function without saving a bool for the success of
	// propagation and passing it in.
	if(on && buffer >= thresh && (noise == 0.0
		|| (noise != 1.0 && r_gen.get_rnum() > noise)))
	{
		value = buffer;
		s.last_visited = last_visited;
		history[0] = true;
	}
	else
		history[0] = false;

	gather_statistics(s);

	if(buffer)
		last_visited = 0;

	buffer = 0;

	return !!value;
}

void Neuron::update_threshold(void)
{
	// To do: either rename this function, or move all of this functionality
	// into pop_temp (after checking that noting will be broken).
	last_visited++;
	history.pop_back();
	history.push_front(false);
}

Synapse::Synapse(const std::valarray<double> p, int t)
	: Node(p), target_age(t), last_visited(-1), window(20.0f) {}

bool Synapse::pop_temp(double noise, Statistics &s)
{
	static auto r_gen = rand_gen<double>(0.0, 1.0);

	// Chemotactic synapse lag is implemented here.
	auto thr = std::max((target_age - last_visited)/window, (double)0.0f);

	if(	on
		// && last_visited > target_age
		&& r_gen.get_rnum() > thr
		&& (noise == 0.0 || (noise != 1.0 && r_gen.get_rnum() > noise)))
	{
		value = buffer;
	}

	if(buffer)
		last_visited = 0;

	buffer = 0;

	return !!value;
}

void Synapse::update_threshold(void)
{
	last_visited++;
}
