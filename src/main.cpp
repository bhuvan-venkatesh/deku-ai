#include <iostream>
#include "key_press.h"
#include "species.h"
#include "genome.h"
#include "neuron.h"
#include "brain.h"
#include "pool.h"
#include "gene.h"
#include "eye.h"


int main(){
	int inputs = 100;
	int outputs = 8;
	Species species(inputs, outputs);
	Genome genome(inputs, outputs);
	Neuron neruon;
	Brain brain(inputs, outputs);
	Pool pool(inputs, outputs);
	Gene gene;
	Eye eye;
	return 0;
}