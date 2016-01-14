#include "neuron.hpp"
#include <cmath>

double Neuron::sigmoid(const double &x) const {
  return 2 / (1 + exp(-4.9 * x)) - 1;
}

void Neuron::clear() {
  weight = 0;
  incoming.clear();
}

Neuron::Neuron(const Neuron &other) { copy(other); }
Neuron::Neuron(Neuron &&other) { swap(other); }
Neuron &Neuron::operator=(Neuron other) {
  swap(other);
  return *this;
}

void Neuron::copy(const Neuron &here) {
  weight = here.weight;
  incoming = here.incoming;
}
void Neuron::swap(Neuron &other) {
  using std::swap;
  swap(weight, other.weight);
  swap(incoming, other.incoming);
}

bool Neuron::save(ofstream &ofs) const {
  ofs << weight << "\n";
  ofs << incoming.size() << "\n";
  for (auto i = incoming.begin(); i != incoming.end(); ++i) {
    ofs << (*i) << "\n";
  }
  return true;
}
bool Neuron::load(ifstream &ifs) {
  ifs >> weight;
  size_t incoming_size;
  ifs >> incoming_size;
  for (size_t i = 0; i < incoming_size; ++i) {
    uint32_t temp;
    ifs >> temp;
    incoming.push_back(temp);
  }
  return true;
}
