#include "genome.hpp"
#include "pool.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib>     /* srand, rand */
#include <ctime>

Genome::Genome(int32_t inputs_, int32_t outputs_):
	 fitness(0),
	 fitness_adjusted(0),
	 max_neuron(0),
	 global_rank(0),
	 inputs(inputs_),
	 outputs(outputs_){
		 //generate_network();
	 	//BI
}

Genome Genome::basic_genome(int32_t inputs, int32_t outputs){
	Genome return_value(inputs, outputs);
	int32_t innovation = 1;

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

vector<bool> Genome::evaluate(vector<int32_t> inputs) {
	if(!validate_input(inputs)){
		return vector<bool>();
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

int32_t Genome::random_neuron(const bool& non_input) const{
	vector<int32_t> neurons;
	for(int32_t i = 0; !non_input && i < inputs; ++i){
		neurons.push_back(i);
	}
	for(int32_t i = 0; i < outputs; ++i){
		neurons.push_back(max_nodes+i);
	}
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(!non_input || !is_input_neuron(i->get_into_neuron())){
			neurons.push_back(i->get_into_neuron());
		}
		if(!non_input || !is_input_neuron(i->get_out_neuron())){
			neurons.push_back(i->get_out_neuron());
		}
	}
	srand(time(NULL));
	return neurons[rand()%(neurons.size())];
}

bool Genome::constains_link(const Gene& link) const {
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(i->same_link(link)){
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
			i->set_weight(i->get_weight()+rand_double()*step*2 - step);
		else
			i->set_weight(rand_double()*4-2);
	}
}

void Genome::link_mutate(const bool& force_bias){
	int32_t neuron1 = random_neuron(false), neuron2 = random_neuron(true);
	if(is_input_neuron(neuron1) && is_input_neuron(neuron2))
		return;
	if(is_input_neuron(neuron2))
		std::swap(neuron1,neuron2);
	Gene link(neuron1, neuron2);
	if(force_bias){
		link.set_into_neuron(inputs-1);
	}
	if(constains_link(link)){
		return;
	}
	link.set_innovation(Pool::innovate()); //TODO: Fix this line with program logic
	link.set_weight(rand_double()*4-2);

	genes.push_back(link);
}

void Genome::node_mutate(){
	if(genes.size() == 0)
		return;
	max_neuron++;
	Gene gene = genes[rand()%genes.size()];
	if(!gene.is_enabled())
		return;
	gene.set_enabled(false);
	Gene inp1(gene);
	inp1.set_out_neuron(max_neuron);
	inp1.set_weight(1.0);
	inp1.set_innovation(Pool::innovate());
	inp1.set_enabled(true);
	genes.push_back(inp1);

	Gene gene2(gene);
	gene2.set_into_neuron(max_neuron);
	gene2.set_innovation(Pool::innovate());
	gene2.set_enabled(true);
	genes.push_back(gene2);
}

void Genome::toggle_enable(const bool& enable){
	vector<Gene*> candidates;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		if(i->is_enabled() == !enable){
			candidates.push_back(&(*i));
		}
	}
	if(candidates.size() == 0)
		return;
	auto& random_candidate = *candidates[rand()%candidates.size()];
	random_candidate.set_enabled(!random_candidate.is_enabled());
}

//TODO: make more efficient if need be
double Genome::disjoint(const Genome& other) const {
	int32_t disjoint_genes=0;
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
	int32_t incident = 0;
	for(auto i = genes.begin(); i != genes.end(); ++i){
		auto gene2 = std::find(other.genes.begin(), other.genes.end(), *i);
		if(gene2 != other.genes.end()){
			sum += std::abs(i->get_weight() - gene2->get_weight());
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

bool Genome::save(ofstream& ofs) const{
	ofs << fitness << "\n";
	ofs << fitness_adjusted << "\n";
	ofs << max_neuron << "\n";
	ofs << global_rank << "\n";
	ofs << inputs << "\n";
	ofs << outputs << "\n";

	ofs << mutation_chance_rates.connection	<< "\n";
	ofs << mutation_chance_rates.disturb	<< "\n";
	ofs << mutation_chance_rates.link	<< "\n";
	ofs << mutation_chance_rates.bias	<< "\n";
	ofs << mutation_chance_rates.node	<< "\n";
	ofs << mutation_chance_rates.enable	<< "\n";
	ofs << mutation_chance_rates.disable	<< "\n";
	ofs << mutation_chance_rates.step	<< "\n";

	ofs << network.size() << "\n";
	for(auto i = network.begin(); i != network.end(); ++i){
			ofs << i->first<< "\n";
			i->second.save(ofs);
	}

	ofs << genes.size() << "\n";
	for(auto i = genes.begin(); i != genes.end(); ++i){
			i->save(ofs);
	}

	return true;
}

bool Genome::load(ifstream& ifs){
	ifs >> fitness
	 	 	>> fitness_adjusted
			>> max_neuron
			>> global_rank
			>> inputs
			>> outputs;

	ifs >> mutation_chance_rates.connection
			>> mutation_chance_rates.disturb
			>> mutation_chance_rates.link
			>> mutation_chance_rates.bias
			>> mutation_chance_rates.node
			>> mutation_chance_rates.enable
			>> mutation_chance_rates.disable
			>> mutation_chance_rates.step;

	size_t network_size;
	ifs >> network_size;
	for(size_t i = 0; i < network_size; ++i){
			Neuron temp;
			int32_t position;
			ifs >> position;
			temp.load(ifs);

			//We are done deserializing one neuron and linking properly
			network[position] = temp;
	}

	size_t genes_size;
	ifs >> genes_size;
	for(size_t i = 0; i < genes_size; ++i){
			Gene temp;
			temp.load(ifs);
			genes.push_back(temp);
	}

	return true;

}


/***************************************************






			Private Member Functions







****************************************************/

void Genome::reset_network_neurons(){
	network.clear();
	for(int32_t i = 0; i < inputs; ++i){
		network[i] = Neuron();
	}
	for(int32_t i = 0; i < outputs; ++i){
		network[max_nodes+i] = Neuron();
	}
}

void Genome::connect_neurons(){
	for(auto j = 0; j != genes.size(); ++j){
		Gene& i = genes[j];
		if(i.is_enabled()){
			initialize_network_neuron(i.get_out_neuron());
			network[i.get_out_neuron()].incoming.push_back(j);
			initialize_network_neuron(i.get_into_neuron());
		}
	}
}

void Genome::initialize_network_neuron(int32_t number){
	if(network.find(number) == network.end()){
		network[number] = Neuron();
	}
}

bool Genome::validate_input(const vector<int32_t>& inputs_) const{
	if(inputs_.size() != inputs){
		std::cout<<"Incorrect number of inputs to the network";
		return false;
	}
	return true;
}

void Genome::update_network_weights(const vector<int32_t>& inputs){
	for(int32_t i = 0; i < this->inputs; ++i){
		network[i].weight = inputs[i];
	}
}

void Genome::evaluate_network(){

	for(auto& pair : network){
		Neuron& current = pair.second;
		double sum = 0;
		for(auto incoming_gene = current.incoming.begin(); incoming_gene != current.incoming.end();
			++incoming_gene){
				Gene& ref = genes[(*incoming_gene)];
			sum += ref.get_weight() * network[ref.get_into_neuron()].weight;
		}
		current.weight = current.sigmoid(sum);
	}
}

vector<bool> Genome::collect_button_commands(){
	vector<bool> output_return;
	for(int32_t i = 0; i < outputs; ++i){
		// > 0 is the test for whether to push button or not
		output_return.push_back(network[max_nodes+i].weight > 0);
	}
	return output_return;
}


Genome Genome::random_gene_swap(const Genome& higher_fitness,
		const Genome& lower_fitness) const{
	Genome child(inputs, outputs);
	std::unordered_map<int32_t, Gene> innov2;
	for(auto i = lower_fitness.genes.begin(); i != lower_fitness.genes.end(); ++i){
		innov2[i->get_innovation()] = *i;
	}
	srand(time(NULL));
	for(auto i = higher_fitness.genes.begin(); i != higher_fitness.genes.end(); ++i){
		Gene gene1 = *i;
		if(innov2.find(gene1.get_innovation()) != innov2.end()){
			Gene gene2 = innov2[gene1.get_innovation()];
			if(rand()%2 && gene2.is_enabled()){
				child.genes.push_back(gene2);
				continue;
			}
		}
		child.genes.push_back(gene1);
	}
	return child;
}

bool Genome::is_input_neuron(int32_t neuron_number) const{
	return neuron_number < inputs;
}

template<typename T>
void Genome::mutate_rate(T& rate){
	rate = (.95+rand()%2*.10263)*rate;
}
