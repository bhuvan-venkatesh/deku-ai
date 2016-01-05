#include <iostream>
#include "key_press.hpp"
#include "species.hpp"
#include "genome.hpp"
#include "neuron.hpp"
#include "brain.hpp"
#include "pool.hpp"
#include "gene.hpp"
#include "eye.hpp"

using cv::KeyPoint;

int main(){
	int inputs = 100;
	int outputs = 8;
	Species species(inputs, outputs);
	Genome genome(inputs, outputs);
	Neuron neruon;
	Brain brain;
	Pool pool(inputs, outputs);
	Gene gene;
	Eye eye;
	eye.draw_keypoints = true;
	getchar();
	return 0;
}
