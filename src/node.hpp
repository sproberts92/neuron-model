#pragma once

#include <iostream>
#include <vector>
#include <valarray>
#include "rand_gen.hpp"

typedef struct
{
	int last_visited = 0;
} Statistics;

class Node
{
public:
	Node();
	Node(const std::valarray<double> p);

	bool get_value(void);
	void set_value(const bool v);

	void turn_on(void);
	void turn_off(void);

	std::valarray<double> get_pos(void);
	std::vector<Node*> get_next(void);
	void push_next(Node &n);

	void clear_signal(void);
	void push_temp_next(void);
	virtual bool pop_temp(double noise, Statistics &s);
	virtual void update_threshold(void);

protected:
	int value;
	int buffer;
	bool on;

private:
	const std::valarray<double> pos;
	std::vector<Node*> next;
};

class Neuron : public Node
{
public:
	Neuron(const std::valarray<double> p, int t);
	bool pop_temp(double noise, Statistics &s);
	void Neuron::update_threshold(void);

private:
	int thresh;
	int last_visited;
};

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
