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
	return ++pool.innovation
}
