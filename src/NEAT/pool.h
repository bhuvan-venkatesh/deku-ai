#ifndef POOL_H
#define POOL_H
#include <cstdint>
#include <unordered_map>

using std::uint32_t;
typedef std::unordered_map<int,int> SpeciesMap;
struct Pool{
public:
	Pool(uint32_t outputs);
	uint32_t innovate();

	Pool(const Pool& other) = default;
	Pool(Pool&& other) = default;
	Pool& operator=(const Pool& other) = default;
	Pool& operator=(Pool&& other) = default;

	SpeciesMap species;
	uint32_t generation;
	uint32_t innovation;
	uint32_t current_species;
	uint32_t current_genome;
	uint32_t current_frame;
	uint32_t max_fitness;
private:
};


#endif