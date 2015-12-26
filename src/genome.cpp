#include "genome.h"
#include "pool.h"
#include <algorithm>
#include <iostream>
#include <cstdlib>     /* srand, rand */
#include <ctime> 

Genome::Genome(uint32_t inputs_, uint32_t outputs_):
	 fitness(0),
	 fitness_adjusted(0),
	 max_neuron(0),
	 global_rank(0),
	 inputs(inputs_),
	 outputs(outputs_){
	 	//BI
	 }

Genome Genome::basic_genome(uint32_t inputs, uint32_t outputs){
	Genome return_value(inputs, outputs);
	uint32_t innovation = 1;

	return_value.max_neuron = inputs-1;
	return_value.mutate();

	return return_value;
}

void Genome::mutate(){
	//TODO: Generated stub, find an appropriate
	//Mutation algorithm
	mutate_rate(mutation_chance_rates.connection);
	mutate_rate(mutation_chance_rates.disturb);
	mutate_rate(mutation_chance_rates.link);
	mutate_rate(mutation_chance_rates.bias);
	mutate_rate(mutation_chance_rates.node);
	mutate_rate(mutation_chance_rates.enable);
	mutate_rate(mutation_chance_rates.disable);
	mutate_rate(mutation_chance_rates.step);

	if(rand_double() < mutation_chance_rates.connection)
		point_mutate();

	auto link_mutate_func = [this](){link_mutate(false);};
	call_mutation_with_chance(mutation_chance_rates.link, link_mutate_func);

	auto link_mutate_func_bias = [this](){link_mutate(true);};
	call_mutation_with_chance(mutation_chance_rates.bias, link_mutate_func_bias);

	auto node_mutate_func = [this](){node_mutate();};
	call_mutation_with_chance(mutation_chance_rates.node, node_mutate_func);

	auto enable_mutate_func = [this](){toggle_enable(true);};
	call_mutation_with_chance(mutation_chance_rates.enable, enable_mutate_func);
	
	auto disable_mutate_func = [this](){toggle_enable(false);};
	call_mutation_with_chance(mutation_chance_rates.disable, disable_mutate_func);

}

void Genome::call_mutation_with_chance(float p, std::function<void ()> func){
	for(; p > 0; --p){
		if(rand_double() < p)
			func();
	}
}

void Genome::generate_network(){
	reset_network_neurons();
	std::sort(genes.begin(), genes.end());
	connect_neurons();
}

unordered_map<string, bool> Genome::evaluate(vector<uint32_t> inputs) {
	if(!validate_input(inputs)){
		return unordered_map<string, bool>();
	}
	update_network_weights(inputs);
	return collect_button_commands();
}

Genome Genome::crossover(const Genome& other) const{
	Genome child(inputs, outputs);
	if(this->fitness > other.fitness){
		child = random_gene_swap(*this, other);
		child.mutation_chance_rates = this->mutation_chance_rates;
	}
	else{
		child = random_gene_swap(other, *this);
		child.mutation_chance_rates = other.mutation_chance_rates;
	}
	child.max_neuron = max_neuron > other.max_neuron?
						max_neuron : other.max_neuron;

 	return child;
}

uint32_t Genome::random_neuron(const bool& non_input) const{
	vector<uint32_t> neurons;
	for(uint32_t i = 0; !non_input && i < inputs; ++i){
		neurons.push_back(i);
	}
	for(uint32_t i = 0; i < outputs; ++i){
		neurons.push_back(max_nodes+i);
	}
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(!non_input || !is_input_neuron(i->into)){
			neurons.push_back(i->into);
		}
		if(!non_input || !is_input_neuron(i->out)){
			neurons.push_back(i->out);
		}
	}
	srand(time(NULL));
	return neurons[rand()%(neurons.size())];
}

bool Genome::constains_link(const Gene& link) const {
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(i->into == link.into && i->out == link.out){
			return true;
		}
	}
	return false;
}

double Genome::rand_double(){
	return double(rand())/RAND_MAX;
}

void Genome::point_mutate(){
	srand(time(NULL));
	const float& step = mutation_chance_rates.step;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(rand_double() < mutation_chance_rates.disturb)
			i->gene_weight += rand_double()*step*2 - step;
		else
			i->gene_weight = rand_double()*4-2;
	}
}

void Genome::link_mutate(const bool& force_bias){
	uint32_t neuron1 = random_neuron(false), neuron2 = random_neuron(true);
	if(is_input_neuron(neuron1) && is_input_neuron(neuron2))
		return;
	if(is_input_neuron(neuron2))
		std::swap(neuron1,neuron2);
	Gene link(neuron1, neuron2);
	if(force_bias){
		link.into = inputs-1;
	}
	if(constains_link(link)){
		return;
	}
	link.innovation = Pool::innovate(); //TODO: Fix this line with program logic
	link.gene_weight = rand_double()*4-2;

	genes.push_back(link);
}

void Genome::node_mutate(){
	if(genes.size() == 0)
		return;
	max_neuron++;
	Gene gene = genes[rand()%genes.size()];
	if(!gene.enabled)
		return;
	gene.enabled = false;
	Gene inp1(gene);
	inp1.out = max_neuron;
	inp1.gene_weight = 1.0;
	inp1.innovation = Pool::innovate();
	inp1.enabled = true;
	genes.push_back(inp1);

	Gene gene2(gene);
	gene2.into = max_neuron;
	gene2.innovation = Pool::innovate();
	gene2.enabled = true;
	genes.push_back(gene2);
}

void Genome::toggle_enable(const bool& enable){
	vector<Gene*> candidates;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(i->enabled == !enable){
			candidates.push_back(&(*i));
		}
	}
	if(candidates.size() == 0)
		return;
	bool& enabled = candidates[rand()%candidates.size()]->enabled;
	enabled = !enabled;
}

//TODO: make more efficient if need be
double Genome::disjoint(const Genome& other) const {
	uint32_t disjoint_genes=0;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(std::find(other.genes.begin(), other.genes.end(), *i) == other.genes.end()){
			disjoint_genes++;
		}
	}
	for(auto i = other.genes.begin(); i != genes.end(); ++i){
		if(std::find(genes.begin(), genes.end(), *i) == other.genes.end()){
			disjoint_genes++;
		}
	}

	return double(disjoint_genes) / std::max(genes.size(), other.genes.size());
}

double Genome::weights(const Genome& other) const{
	double sum=0;
	uint32_t incident = 0;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		auto gene2 = std::find(other.genes.begin(), other.genes.end(), *i);
		if(gene2 != other.genes.end()){
			sum += std::abs(i->gene_weight - gene2->gene_weight);
			incident++;
		}
	}
	return sum / incident;
}

bool Genome::same_species(const Genome& other) const{
	double delta_d = delta_disjoint*disjoint(other);
	double delta_w = delta_weights*weights(other);
	return delta_d + delta_w < delta_threshold;
}



/***************************************************

			




			Private Member Functions







****************************************************/

void Genome::reset_network_neurons(){
	network.clear();
	for(uint32_t i = 0; i < inputs; ++i){
		network[i] = Neuron();
	}
	for(uint32_t i = 0; i < outputs; ++i){
		network[max_nodes+i] = Neuron();
	}
}

void Genome::connect_neurons(){
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(i->enabled){
			initialize_network_neuron(i->out);
			network[i->out].incoming.push_back(*i);
			initialize_network_neuron(i->into);
		}
	}
}

void Genome::initialize_network_neuron(uint32_t number){
	if(network.find(number) == network.end()){
		network[number] = Neuron();
	}
}

bool Genome::validate_input(const vector<uint32_t>& inputs_) const{
	if(inputs_.size() != inputs){
		std::cout<<"Incorrect number of inputs to the network";
		return false;
	}
	return true;
}

void Genome::update_network_weights(const vector<uint32_t>& inputs){
	for(uint32_t i = 0; i < this->inputs; ++i){
		network[i].weight = inputs[i];
	}
}

void Genome::evaluate_network(){

	for(auto& pair : network){
		Neuron& current = pair.second;
		if(current.incoming.size() > 0){
			double sum = 0;
			for(auto incoming_gene = current.incoming.begin(); incoming_gene != current.incoming.end();
				++incoming_gene){
				sum += incoming_gene->gene_weight * 
						network[incoming_gene->into].weight;
			}
			current.weight = current.sigmoid(sum);
		}
	}
}

std::unordered_map<string, bool> Genome::collect_button_commands(){
	unordered_map<string, bool> output_return;
	for(uint32_t i = 0; i < outputs; ++i){
		//TODO:UPDATE
		string button = "P1 ";
		// > 0 is the test for whether to push button or not
		output_return[button] = network[max_nodes+i].weight > 0;
	}
	return output_return;
}


Genome Genome::random_gene_swap(const Genome& higher_fitness, 
		const Genome& lower_fitness) const{
	Genome child(inputs, outputs);
	std::unordered_map<uint32_t, Gene> innov2;
	for(auto i = lower_fitness.genes.begin(); i != lower_fitness.genes.end(); ++i){
		innov2[i->innovation] = *i;
	}
	srand(time(NULL));
	for(auto i = higher_fitness.genes.begin(); i != higher_fitness.genes.end(); ++i){
		Gene gene1 = *i;
		if(innov2.find(gene1.innovation) != innov2.end()){
			Gene gene2 = innov2[gene1.innovation];
			if(rand()%2==0 && gene2.enabled){
				child.genes.push_back(gene2);
				continue;
			}
		}
		child.genes.push_back(gene1);
	}
	return child;
}

bool Genome::is_input_neuron(uint32_t neuron_number) const{
	return neuron_number < inputs;
}

template<typename T>
void Genome::mutate_rate(T& rate){
	rate = (.95+rand()%2*.10263)*rate;
}