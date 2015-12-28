#include "gene.h"

Gene::Gene(): into(0), out(0), 
	gene_weight(0), innovation(0), enabled(true){
		//Left blank intentionally
	}
Gene::Gene(const int32_t& into_, const int32_t& out_): 
	into(into_), out(out_), gene_weight(0), 
	innovation(0), enabled(true){

}