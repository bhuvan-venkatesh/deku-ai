#include "genome.h"

Genome::Genome():
	 fitness(0),
	 fitness_adjusted(0),
	 max_neuron(0),
	 global_rank(0){
	 	//BI
	 }

Genome Genome::basic_genome(){
	Genome return_value;
	uint32_t innovation = 1;
	return_value.max_neuron = inputs;
	return_value.mutate(inputs);

	return return_value;
}

void mutate(uint32_t inputs){
	//TODO: Generated stub, find an appropriate
	//Mutation algorithm


	//This is technically a correct impelmentation
}