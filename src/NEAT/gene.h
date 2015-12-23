#ifndef GENE_H
#define GENE_H
#include <cstdint>

using std::uint32_t;

struct Gene{
public:
	uint32_t into;
	uint32_t out;
	uint32_t gene_weight;
	uint32_t innovation;
	bool enabled;

	Gene();
	Gene(const Gene& swap) = default;
	Gene(Gene&& rhs) = default;
	Gene& operator=(const Gene& rhs) = default;
	Gene& operator=(Gene&& rhs) = default;

	bool operator<(const Gene& rhs)
	{
		return out < rhs.out;
	}
private:
	//BI
};


#endif