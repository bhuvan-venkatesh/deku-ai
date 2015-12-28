#include <iostream>
#include "key_press.h"
#include "species.h"
#include "genome.h"
#include "neuron.h"
#include "brain.h"
#include "pool.h"
#include "gene.h"
#include "eye.h"

using cv::KeyPoint;
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
	eye.draw_keypoints = true;
	std::vector<cv::KeyPoint> points = eye.analyze_screen();
	std::sort(points.begin(), points.end(), [](const KeyPoint& lhs, const KeyPoint& rhs){
		return lhs.response > rhs.response;
	});
	for(auto i = points.begin(); i != points.end(); ++i)
		std::cout<<i->pt.x<<" "<<i->pt.y
					<<" "<<i->size<<" "<<i->response<<std::endl;
	getchar();
	return 0;
}