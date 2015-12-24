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
	Gene(const uint32_t& into_, const uint32_t& out_);
	Gene(const Gene& swap) = default;
	Gene(Gene&& rhs) = default;
	Gene& operator=(const Gene& rhs) = default;
	Gene& operator=(Gene&& rhs) = default;

	bool operator<(const Gene& rhs) const
	{
		return out < rhs.out;
	}

	bool operator==(const Gene& rhs) const{
		return innovation == rhs.innovation;
	}
private:
	//BI
};


#endif