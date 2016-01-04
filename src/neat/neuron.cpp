#include "neuron.hpp"
#include <cmath>

double Neuron::sigmoid(const double& x) const {
	return 2/(1+exp(-4.9*x))-1;
}

bool Neuron::save(ofstream& ofs) const{
	ofs << weight << "\n";
	ofs << incoming.size() << "\n";
	for(auto i = incoming.begin(); i != incoming.end();++i){
		ofs << (*i)->save(ofs) << "\n";
	}
	return true;
}
bool Neuron::load(ifstream& ifs) {
	ifs >> weight;
	size_t incoming_size;
	ifs >> incoming_size;
	for(size_t i = 0; i < incoming_size; ++i){
		Gene* regret = new Gene();
		regret->load(ifs);
		incoming.push_back(regret);
	}
	return true;
}
