#pragma once

#include <cstdint>
#include "serial.hpp"
using std::int32_t;

struct Gene: public Serial
{
public:
	/* With alignment -
	 20 bytes*/

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

	bool same_link(const Gene& other) const {
		return into == other.into && out == other.out;
	}

	bool save(ofstream& ofs) const;
  bool load(ifstream& ifs);
	virtual ~Gene() {}

	int32_t into;
	int32_t out;
	int32_t weight;
	int32_t innovation;
	bool enabled;
private:
	char wastage[7];
};
