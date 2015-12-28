#ifndef GENE_H
#define GENE_H

#include <cstdint>

using std::int32_t;

struct Gene{
public:
	int32_t into;
	int32_t out;
	int32_t gene_weight;
	int32_t innovation;
	bool enabled;

	Gene();
	Gene(const int32_t& into_, const int32_t& out_);
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