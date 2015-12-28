#include "species.h"
#include <ctime>
#include <cstdlib>

Species::Species(int32_t inputs_, int32_t outputs_):
	top_fitness(0),
	staleness(0),
	average_fitness(0),
	inputs(inputs_),
	outputs(outputs_){
	//BI
}

void Species::calculate_average_fitness(){
	int32_t total = 0;
	for(auto g = genomes.begin(); g != genomes.end(); ++g){
		total += g->global_rank;
	}
	average_fitness = total / double(genomes.size());


}

Genome Species::breed_child(){
	srand(time(NULL));
	Genome child(inputs, outputs);
	if(rand()/(double)RAND_MAX < crossover_chance){
		auto g1 = genomes[rand()%genomes.size()];
		auto g2 = genomes[rand()%genomes.size()];
		child = g1.crossover(g2);
	}
	else{
		child = genomes[rand()%genomes.size()];
	}

	child.mutate();

	return child;
}
