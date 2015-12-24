#include "pool.h"
Pool::innovation = 0;

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

void Pool::remove_stale_species(){
	rank_globally();
	for(uint32_t i = 0; i < species.size(); ++i){
		Species& s = species[i];
		std::sort(s->genomes.begin(), s->genomes.end(), std::greater<Genome>());
		if(s->genomes[0].fitness > s->top_fitness){
			s->top_fitness = s->genomes[0].fitness;
			s->staleness = 0;
		}
		else
			s->staleness++;
		if(s->staleness > stale_species && s->top_fitness <= max_fitness){
			std::erase(std::remove(species.begin(), species.end(), s), species.end());
			--i;
		}
	}
}

void Pool::remove_weak_species(){

	auto sum = calculate_average_fitness();
	for(uint32_t i = 0; i < species.size(); ++i){
		Species& s= species[i];
		int32_t breed = int(species.average_fitness / sum * population);
		if(breed < 1){
			std::erase(std::remove(species.begin(), species.end(), s), species.end());
			--i;
		}
	}

}

void Pool::add_to_species(const Genome& child){
	bool found_species = false;
	for(auto i = species.begin(); i != species.end() && !found_species; ++i){
		if(child.same_species(i->genomes[0])){
			species.genomes.push_back(child);
			found_species = true;
		}
	}

	if(!found_species){
		Species child;
		child.genomes.push_back(child);
		species.push_back(child);
	}
}


void Pool::new_generation(){
	cull_species(false);
	remove_stale_species();
	rank_globally();
	for(auto s = species.begin(); s != species.end(); ++s){
		s.calculate_average_fitness();
	}
	remove_weak_species();
	uint32_t sum = calculate_average_fitness();
	vector<Genome> children;

	for(auto s = species.begin(); s != species.end(); ++s){
		s.calculate_average_fitness();
		int32_t breed = int(species.average_fitness / sum * population);
		for(uint32_t i = 0; i < breed; ++i){
			children.push_back(s->breed_child());
		}
	}
	cull_species(true);
	while(children.size() + species.size() < population){
		children.push_back(species[rand()%species.size()].breed_child());
	}

	for(auto c = children.begin(); c != children.end(); ++c){
		add_to_species(*c);
	}
	generation++;

	//TODO:Backup data
}
