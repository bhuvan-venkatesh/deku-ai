#ifndef GENOME_H
#define GENOME_H
#include <cstdint>
#include <vector>
#include <unordered_map>
#include "neuron.h"

#define max_nodes 1000000
#define POOL_PLACE 0
#define delta_disjoint 2.0
#define delta_weights 0.4
#define delta_threshold 1.0
#define stale_species 15



typedef std::unordered_map<std::uint32_t,Neuron> NetworkMap;
//In case there is a need to change later
using std::vector;
using std::uint32_t;
using std::unordered_map;
using std::string;

namespace action_chances{
		const static float 
			mutation_connect 	= 0.25,
			disturb 			= 0.9,
			link 				= 2.0,
			node 				= 0.5,
			bias_mutation 		= 0.4,
			step_size 			= 0.1,
			disable_mutation 	= 0.4,
			enable_mutation 	= 0.2;
	};



class Genome{
public:
	//TODO: Find flexible values, converging


	vector<Gene> genes;
	typedef struct {
		float connection	= action_chances::mutation_connect;
		float disturb		= action_chances::disturb;
		float link			= action_chances::link;
		float bias 			= action_chances::bias_mutation;
		float node			= action_chances::node;
		float enable 		= action_chances::enable_mutation;
		float disable 		= action_chances::disable_mutation;
		float step 			= action_chances::step_size;
	} chances;
	chances mutation_chance_rates;
	NetworkMap network;
	vector<string> button_names;

	uint32_t fitness;
	uint32_t fitness_adjusted;
	uint32_t max_neuron;
	uint32_t global_rank;
	uint32_t inputs;
	uint32_t outputs;

	Genome(uint32_t inputs_, uint32_t outputs_);
	Genome(const Genome& other) = default;
	Genome(Genome&& other) = default;
	Genome& operator= (const Genome& other) = default;
	Genome& operator= (Genome&& other) = default;

	static Genome basic_genome(uint32_t inputs, uint32_t outputs);

	void mutate();
	void generate_network();
	unordered_map<string, bool> evaluate(vector<uint32_t> inputs);
	Genome crossover(const Genome& other) const;
	uint32_t random_neuron(const bool& non_input) const;
	bool constains_link(const Gene& link) const;
	void point_mutate();
	void link_mutate(const bool& force_bias);
	void node_mutate();
	void toggle_enable(const bool& enabled);
	double disjoint(const Genome& other);
	double weights(const Genome& other);

	bool operator< (const Genome& other){
		return fitness < other.fitness;
	}

	bool operator> (const Genome& other){
		return fitness > other.fitness;
	}
private:
	void reset_network_neurons();
	void connect_neurons();
	bool validate_input(const vector<uint32_t>& inputs) const;
	void initialize_network_neuron(uint32_t number);
	void update_network_weights(const vector<uint32_t>& inputs);
	void evaluate_network();
	unordered_map<string, bool> collect_button_commands();

	Genome random_gene_swap(const Genome& higher_fitness, 
		const Genome& lower_fitness) const;
	double rand_double();
	bool is_input_neuron(uint32_t neuron_number) const;
	
	template<typename T>
	void mutate_rate(T& rate);
};

#endif