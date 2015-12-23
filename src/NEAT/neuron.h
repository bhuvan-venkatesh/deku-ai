#ifndef NEURON_H
#define NEURON_H

#include "gene.h"
#include <vector>

using std::vector;
struct Neuron{
	vector<gene> incoming;
	double weight;

	Neuron(): weight(0) {};
	Neuron(const Neuron& other) = default;
	Neuron(Neuron&& other) = default;
	Neuron& operator= (const Neuron& other) = default;
	Neuron& operator= (Neuron&& other) = default;
};

#endif