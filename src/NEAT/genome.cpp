#include "genome.h"
#include <algorithm>
#include <iostream>

Genome::Genome(uint32_t inputs_, uint32_t outputs_):
	 fitness(0),
	 fitness_adjusted(0),
	 max_neuron(0),
	 global_rank(0)
	 inputs(inputs_)
	 outputs(outputs_){
	 	//BI
	 }

Genome Genome::basic_genome(uint32_t inputs){
	Genome return_value(inputs);
	uint32_t innovation = 1;
	return_value.max_neuron = inputs;
	return_value.mutate();

	return return_value;
}

void Genome::mutate(){
	//TODO: Generated stub, find an appropriate
	//Mutation algorithm


	//This is technically a correct impelmentation
}

void Genome::generate_network(){
	network.clear();
	NetworkMap& network = this->network;
	for(uint32_t i = 0; i < inputs; ++i){
		network[i] = Neuron();
	}
	for(uint32_t i = 0; i < outputs; ++i){
		network[max_nodes+o] = Neuron();
	}

	std::sort(genome.genes.begin(), genome.genes.end());

	for(auto i = genome.genes.begin(); i != genome.genes.end(); ++i){
		if(i->enabled){
			if(network.find(i->out) == network.end()){
				network[i->out] = Neuron();
			}
			network[i->out].incoming.push_back(*i);
			if(network.find(i->into) == network.end()){
				network[i->into] = Neuron();
			}
		}
	}
}
unordered_map<string, bool> evaluate(
		vector<uint32_t> inputs){
	unordered_map<string, bool> output_return;
	if(inputs.size() != inputs){
		std::cout<<"Incorrect number of inputs to the network";
		return output_return;
	}

	for(uint32_t i = 0; i < inputs; ++i){
		network[i].gene_weight = inputs[i];
	}

	for(auto& pair : network){
		Neuron& current = pair.second;
		if(current.incoming.size() > 0){
			double sum = 0;
			for(Gene& incoming_gene = current.incoming.begin(); 
				incoming_gene != current.incoming.end();
				++incoming_gene){
				sum += incoming_gene.gene_weight * network[incoming.into].weight;
			}
			current.weight = current.sigmoid(sum);
		}
	}

	for(uint32_t i = 0; i < outputs; ++i){
		string button = "P1 "+button_names[i];
		output_return[button] = network[max_nodes+i].weight > 0;
	}
	return output_return;
}