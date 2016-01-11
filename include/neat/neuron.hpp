#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "gene.hpp"
#include "serial.hpp"

using std::vector;
using std::int32_t;

struct Neuron: public Serial{
	vector<int32_t> incoming;
	double weight;

	double sigmoid(const double& x) const;

	bool save(ofstream& ofs) const;
	bool load(ifstream& ifs);

	Neuron(): weight(0) {};
	Neuron(const Neuron& other) = default;
	Neuron(Neuron&& other) = default;
	Neuron& operator= (const Neuron& other) = default;
	Neuron& operator= (Neuron&& other) = default;
};

#endif
