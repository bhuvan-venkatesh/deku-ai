#include "pool.h"

Pool(uint32_t outputs): 
	generation(0),
	current_species(1),
	current_genome(1),
	current_frame(0),
	max_fitness(0)
	innovation(outputs){
		//BI
}
uint32_t innovate(){
	return ++pool.innovation;
}

void Pool::rank_globally(){
	/*You have to use pointers or else the original argument
		Will not be changed and you cannot use references in templates*/

	vector<Genome*> global;
	for(auto species = species.begin(); species != species.end(); ++species){
		for(auto genome = species.genomes.begin(); genome != species.genomes.end(); ++genome){
			global.push_back(&(*i));
		}
	}
	std::sort(global.begin(), global.end(), [](Genome* a, Genome* b){
		return *a < *b;
	});
	for(uint32_t i = 0; i < global.size(); ++i){
		global[i]->global_rank = i+1;
	}
}

uint32_t Pool::calculate_average_fitness(){
	uint32_t total = 0;
	for(auto s = species.begin(); s != species.end(); ++s){
		total += species.average_fitness();
	}
	return total;
}

void Pool::cull_species(bool cut_to_one){
	for(auto s = species.begin(); s != species.end(); ++s){
		std::sort(s->genomes.begin(), s->genomes.end(), std::greater<Genome>());
		uint32_t remaining = std::ceil(species.size()/2);
		if(cut_to_one){
			remaining = 1;
		}
		while(remaining-- > 0)
			s->genomes.pop_back();
	}
}
