#ifndef SPECIES_H
#define SPECIES_H
#include <cstdint>
#include <vector>
#include "genome.h"
#define crossover 0.75,
using std::uint32_t;
using std::vector;
struct Species{
public:
	uint32_t top_fitness;
	uint32_t staleness;
	uint32_t average_fitness;
	vector<Genome> genomes;

	Species();
	Species(const Species& other) = default;
	Species(Species&& other) = default;
	Species& operator= (const Species& other) = default;
	Species& operator= (Species&& other) = default;

	bool operator< (const Species& other) const{
		return average_fitness < other.average_fitness;
	}

	void calculate_average_fitness();
	Genome breed_child();
private:
};

#endif