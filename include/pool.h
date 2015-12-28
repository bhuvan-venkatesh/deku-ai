#ifndef POOL_H
#define POOL_H
#include <cstdint>
#include <unordered_map>
#include "species.h"
//#include <cstdlib>
//#include <cmath>
#define stale_species 15
#define population 300

using std::int32_t;
using std::vector;
struct Pool{
public:
	Pool(int32_t inputs_, int32_t outputs_);
	static int32_t innovate();

	Pool(const Pool& other) = default;
	Pool(Pool&& other) = default;
	Pool& operator=(const Pool& other) = default;
	Pool& operator=(Pool&& other) = default;

	vector<Species> species;
	int32_t generation;
	static int32_t innovation;
	int32_t current_species;
	int32_t current_genome;
	int32_t current_frame;
	int32_t max_fitness;
	int32_t inputs, outputs;

	void rank_globally();
	int32_t calculate_average_fitness();
	void cull_species(bool cut_to_one);
	void remove_stale_species();
	void remove_weak_species();
	void add_to_species(const Genome& child);
	void new_generation();
private:
};


#endif