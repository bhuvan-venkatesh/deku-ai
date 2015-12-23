#ifndef GENOME_H
#define GENOME_H
#include <cstdint>
#include <vector>
#include <unordered_map>
#include "neuron.h"

typedef std::unordered_map<std::uint32_t,Neuron> network_map;
//In case there is a need to change later
using std::vector;
using std::uint32_t;
struct Genome{
public:
	//TODO: Find flexible values, converging
	namespace action_chances{
		const static float 
			mutation_connect 	= 0.25,
			disturb 			= 0.9,
			crossover 			= 0.75,
			link 				= 2.0,
			node 				= 0.5,
			bias_mutation 		= 0.4,
			step_size 			= 0.1,
			disable_mutation 	= 0.4,
			enable_mutation 	= 0.2;
	};

	vector<Gene> genes;
	typedef struct {
		float connection	= action_chances::mutation_connect;
		float link			= action_chances::link;
		float bias 			= action_chances::bias_mutation;
		float node			= action_chances::node;
		float enable 		= action_chances::enable_mutation;
		float disable 		= action_chances::disable_mutation;
		float step 			= action_chances::step_size;
	} chances;
	chances mutation_chance_rates;
	network_map network;

	uint32_t fitness;
	uint32_t fitness_adjusted;
	uint32_t max_neuron;
	uint32_t global_rank;

	Genome();
	Genome(const Genome& other) = default;
	Genome(Genome&& other) = default;
	Genome& operator= (const Genome& other) = default;
	Genome& operator= (Genome&& other) = default;

	static Genome basic_genome(uint32_t inputs);

	void mutate(uint32_t inputs);
private:
};

#endif