#pragma once

#include <iostream>
#include <vector>
#include <valarray>
#include <array>
#include <deque>
#include <numeric>
#include "rand_gen.hpp"

// POD class for containment of statistics gathered during signal propagation.
struct Statistics
{
	int last_visited = 0;
	std::vector<int> pattern_found;
};

// A basic node class from which the network will be build. Used without
// specialisation for Schwann cells, and with specialisation for Soma and
// Synapse nodes.
class Node
{
public:
	Node(const std::valarray<double> p);

	bool get_value(void);
	void set_value(const bool v);

	// Turning a node off prevents propagation of a signal through the node.
	void turn_on(void);
	void turn_off(void);

	std::valarray<double> get_pos(void);
	std::vector<Node*> get_next(void);

	// Wrapper for next.push_back(&n).
	void push_next(Node &n);

	// Clear all signals in the node. In most cases this will mean zeroing the
	// value and buffer variables, but could mean something else if other
	// values are added.
	void clear_signal(void);

	// Propagate signal from the value field in the current node to the buffer
	// in each of the nodes pointed to in the `next` vector.
	void push_temp_next(void);

	// Pop the signal from the buffer into the actual value varible. Returns
	// true if the buffer had contents that were popped, and false otherwise.
	virtual bool pop_temp(double noise, Statistics &s);

	// Update propagation thresholds where relevant (in derived classes).
	virtual void update_threshold(void);

protected:
	int value;
	int buffer;
	bool on;

private:
	const std::valarray<double> pos;

	// Todo: replace with smart pointers.
	std::vector<Node*> next;
};

// A specialisation of the Node class for use as a neuron body (soma) cell.
// The primary purpose of this specialisation is to change propagation rules
// for this type of cell and to add the ability to gather additional statistics
// beyond the base Node class on signal throughput.
class Neuron : public Node
{
public:
	Neuron(const std::valarray<double> p, int t);
	bool pop_temp(double noise, Statistics &s);
	void Neuron::update_threshold(void);

private:
	int thresh;

	// Counts the number of timesteps since a signal was last present on this
	// neuron.
	int last_visited;

	// Stores the history of the state of the node (signal or no signal) over
	// the last n timesteps.
	std::deque<bool> history;

	// Gathers any statistics on the current state of signals in the neuron. At
	// the moment this is the historical pattern search but anything else
	// needed can be added here too.
	void gather_statistics(Statistics &s);
};

// A specialisation of the Node class for use as a synapse. The primary purpose
// if this specialisation is to change propagation rules to add plasticity and
// more complicated conditional firing.
class Synapse : public Node
{
public:
	Synapse(const std::valarray<double> p, int t);
	bool pop_temp(double noise, Statistics &s);
	void update_threshold(void);

private:
	int target_age;
	int last_visited;
	double window;
};
