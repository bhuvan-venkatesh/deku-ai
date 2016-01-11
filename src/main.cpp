#include <iostream>
#include "brain.hpp"

using cv::KeyPoint;

int main(){
	Brain brain;
	brain.initialize_run();
	while(true){
		std::cout<<1<<std::endl;
		brain.controller.a();
		std::cout<<1<<std::endl;
		brain.controller.execute();
	}
	getchar();
	return 0;
}
