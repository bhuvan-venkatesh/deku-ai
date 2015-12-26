#ifndef SPECIES_H
#define SPECIES_H
#include <cstdint>
#include <vector>
#include "genome.h"
#define crossover_chance 0.75
using std::uint32_t;
using std::vector;
struct Species{
public:
	uint32_t top_fitness;
	uint32_t staleness;
	uint32_t average_fitness;
	uint32_t inputs, outputs;
	vector<Genome> genomes;

	Species(uint32_t inputs_, uint32_t outputs_);
	Species(const Species& other) = default;
	Species(Species&& other) = default;
	Species& operator= (const Species& other) = default;
	Species& operator= (Species&& other) = default;

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

	void calculate_average_fitness();
	Genome breed_child();
private:
};

#endif