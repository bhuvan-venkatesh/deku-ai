#ifndef NEURON_H
#define NEURON_H

#include "gene.h"
#include <vector>
#include <unordered_map>
#include <cstdint>

using std::vector;
using std::int32_t;
struct Neuron{
	vector<Gene> incoming;
	double weight;

	double sigmoid(const double& x) const;

	Neuron(): weight(0) {};
	Neuron(const Neuron& other) = default;
	Neuron(Neuron&& other) = default;
	Neuron& operator= (const Neuron& other) = default;
	Neuron& operator= (Neuron&& other) = default;
};

#endif