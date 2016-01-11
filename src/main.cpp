#include <iostream>
#include "brain.hpp"

using cv::KeyPoint;

int main(){
	Brain brain;
	brain.initialize_run();
	while(true){
		brain.play();
	}
	getchar();
	return 0;
}
