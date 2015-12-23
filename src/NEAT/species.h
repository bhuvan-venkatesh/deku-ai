#ifndef SPECIES_H
#define SPECIES_H
#include <cstdint>
#include <vector>
#include "Genome.h"

using std::uint32_t;

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
private:
};

#endif