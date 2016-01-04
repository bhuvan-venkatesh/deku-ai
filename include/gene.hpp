#ifndef GENE_H
#define GENE_H

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

	inline int32_t get_into_neuron() const{
		return into;
	}

	inline void set_into_neuron(const int32_t& into_){
		into = into_;
	}

	inline int32_t get_out_neuron() const{
		return out;
	}

	inline void set_out_neuron(const int32_t& out_){
		out = out_;
	}

	inline int32_t get_weight() const{
		return weight;
	}
	inline void set_weight(const int32_t& weight_){
		weight = weight_;
	}

	inline int32_t get_innovation() const{
		return innovation;
	}

	inline void set_innovation(const int32_t& innov_){
		innovation = innov_;
	}

	inline bool is_enabled() const{
		return enabled;
	}

	inline void set_enabled(const bool& enab_){
		enabled = enab_;
	}
private:
	int32_t into;
	int32_t out;
	int32_t weight;
	int32_t innovation;
	bool enabled;
};


#endif
