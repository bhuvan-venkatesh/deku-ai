#pragma once

#include <cstdint>
#include <vector>
#include "genome.hpp"
#include "serial.hpp"
#define crossover_chance 0.75

using std::int32_t;
using std::vector;

struct Species : public Serial{
public:
	int32_t top_fitness;
	int32_t staleness;
	int32_t average_fitness;
	int32_t inputs, outputs;
	vector<Genome> genomes;

	bool operator< (const Species& other) const{
		return average_fitness < other.average_fitness;
	}

	bool operator< (const Species& other){
		return average_fitness < other.average_fitness;
	}

	bool operator== (const Species& other) const{
		return top_fitness == other.top_fitness &&
		staleness == other.staleness &&
		average_fitness == other.average_fitness &&
		genomes == other.genomes;
	}
	bool operator== (const Species& other){
		return top_fitness == other.top_fitness &&
		staleness == other.staleness &&
		average_fitness == other.average_fitness &&
		genomes == other.genomes;
	}

	bool save(ofstream& ofs) const;
	bool load(ifstream& ifs);
	virtual ~Species() {}

	void calculate_average_fitness();
	Genome breed_child();

	Species(int32_t inputs_, int32_t outputs_);
	Species(const Species& other) = default;
	Species(Species&& other) = default;
	Species& operator= (const Species& other) = default;
	Species& operator= (Species&& other) = default;
private:
};
