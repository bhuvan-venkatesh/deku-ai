#ifndef POOL_H
#define POOL_H
#include <cstdint>
#include <unordered_map>
#define stale_species 15
#define population 300

using std::uint32_t;
using std::vector;
struct Pool{
public:
	Pool(uint32_t outputs);
	static uint32_t innovate();

	Pool(const Pool& other) = default;
	Pool(Pool&& other) = default;
	Pool& operator=(const Pool& other) = default;
	Pool& operator=(Pool&& other) = default;

	vector<Species> species;
	uint32_t generation;
	static uint32_t innovation;
	uint32_t current_species;
	uint32_t current_genome;
	uint32_t current_frame;
	uint32_t max_fitness;

	void rank_globally();
	uint32_t calculate_average_fitness();
	void cull_species(bool cut_to_one);
	void remove_stale_species();
	void remove_weak_species();
	void add_to_species(const Genome& child);
	void new_generation();
private:
};


#endif