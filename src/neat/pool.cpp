#include "pool.hpp"
#include <algorithm>

int32_t Pool::innovation = 0;

Pool::Pool(int32_t inputs_, int32_t outputs_):
	generation(0),
	current_species(0),
	current_genome(0),
	current_frame(0),
	max_fitness(0),
	inputs(inputs_),
	outputs(outputs_){
		//BI
		Species first_species(inputs_, outputs_);
		Genome first_genome(inputs_, outputs_);
		first_species.genomes.push_back(first_genome);
		species.push_back(first_species);
}
int32_t Pool::innovate(){
	return ++Pool::innovation;
}

void Pool::rank_globally(){
	/*You have to use pointers or else the original argument
		Will not be changed and you cannot use references in templates*/

	vector<Genome*> global;
	for(auto s = species.begin(); s != species.end(); ++s){
		for(auto genome = s->genomes.begin(); genome != s->genomes.end(); ++genome){
			global.push_back(&(*genome));
		}
	}
	std::sort(global.begin(), global.end(), [](Genome* a, Genome* b){
		return *a < *b;
	});
	for(int32_t i = 0; i < global.size(); ++i){
		global[i]->global_rank = i+1;
	}
}

int32_t Pool::calculate_average_fitness(){
	int32_t total = 0;
	for(auto s = species.begin(); s != species.end(); ++s){
		total += s->average_fitness;
	}
	return total;
}

void Pool::cull_species(bool cut_to_one){
	for(auto s = species.begin(); s != species.end(); ++s){
		std::sort(s->genomes.begin(), s->genomes.end(), std::greater<Genome>());
		int32_t remaining = std::ceil(species.size()/2);
		if(cut_to_one){
			remaining = 1;
		}
		while(remaining-- > 0)
			s->genomes.pop_back();
	}
}

void Pool::remove_stale_species(){
	rank_globally();
	for(int32_t i = 0; i < species.size(); ++i){
		Species& s = species[i];
		std::sort(s.genomes.begin(), s.genomes.end(), std::greater<Genome>());
		if(s.genomes[0].fitness > s.top_fitness){
			s.top_fitness = s.genomes[0].fitness;
			s.staleness = 0;
		}
		else
			s.staleness++;
		if(s.staleness > stale_species && s.top_fitness <= max_fitness){
			species.erase(std::find(species.begin(), species.end(), s));
			--i;
		}
	}
}

void Pool::remove_weak_species(){

	auto sum = calculate_average_fitness();
	for(int32_t i = 0; i < species.size(); ++i){
		Species& s= species[i];
		int32_t breed = int(s.average_fitness / sum * population);
		if(breed < 1){
			species.erase(std::find(species.begin(), species.end(), s));
			--i;
		}
	}

}

void Pool::add_to_species(const Genome& child){
	bool found_species = false;
	for(auto i = species.begin(); i != species.end() && !found_species; ++i){
		if(child.same_species(i->genomes[0])){
			i->genomes.push_back(child);
			found_species = true;
		}
	}

	if(!found_species){
		Species childe(inputs, outputs);
		childe.genomes.push_back(child);
		species.push_back(childe);
	}
}


void Pool::new_generation(){
	cull_species(false);
	remove_stale_species();
	rank_globally();
	for(auto s = species.begin(); s != species.end(); ++s){
		s->calculate_average_fitness();
	}
	remove_weak_species();
	int32_t sum = calculate_average_fitness();
	vector<Genome> children;

	for(auto s = species.begin(); s != species.end(); ++s){
		s->calculate_average_fitness();
		int32_t breed = int(s->average_fitness / sum * population);
		for(int32_t i = 0; i < breed; ++i){
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

}

vector<bool> Pool::evaluate(const vector<int32_t>& inputs){

	Species& current_spec = species[current_species];
	Genome& current_gen = current_spec.genomes[current_genome];
	current_gen.generate_network();
	return current_gen.evaluate(inputs);
}


bool Pool::save(ofstream& ofs) const{
	ofs << innovation << "\n"
			<< generation << "\n"
			<< current_species << "\n"
	 		<< current_genome << "\n"
			<< current_frame << "\n"
	 		<< max_fitness << "\n"
			<< inputs << "\n" << outputs << "\n";

	ofs << species.size() << "\n";
	for(auto i = species.begin(); i != species.end(); ++i){
		i->save(ofs);
	}
	return true;

}
bool Pool::load(ifstream& ifs){
	ifs >> innovation
		>> generation
		>> current_species
		>> current_genome
		>> current_frame
		>> max_fitness
		>> inputs
		>> outputs;
	size_t stuff;
	ifs >> stuff;
	species.clear();
	for(size_t i = 0; i < stuff; ++i){
		Species temp(inputs, outputs);
		temp.load(ifs);
		species.push_back(temp);
	}
	return true;
}

void Pool::set_top(){
	int32_t maxfitness = 0, maxs, maxg
	for(auto s = species.begin(), size_t s_ = 0; s != species.end(); ++s, ++s_){
		for(auto g = s->genomes.begin(),size_t g_ = 0; g != s->genomes.end(); ++g, ++g_){
			if(g->fitness > maxfitness){
				maxfitness = g->fitness;
				maxs = s_;
				maxg = g_;
			}
		}
	}

	current_spec = maxs
	current_genome = maxg
	max_fitness = maxfitness
	current_frame++;
}

void Pool::next_genome(){
	current_genome++;
	if(current_genome) > species[current_species].genomes.size(){
		current_genome = 0;
		current_species++;
		if(current_species > species.size(){
			new_generation()
			current_species = 0;
		}
	}
}
bool Pool::fitness_measured() const{
	return species[current_species].genomes[current_genome].fitness != 0;
}
