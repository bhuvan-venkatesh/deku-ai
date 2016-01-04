#include "neuron.hpp"
#include <cmath>

double Neuron::sigmoid(const double& x) const {
	return 2/(1+exp(-4.9*x))-1;
}
