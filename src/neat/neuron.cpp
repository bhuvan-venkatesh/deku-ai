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

bool Neuron::evaluate() const { weight > 0; }

void Neuron::propogate(const std::unordered_map<int32_t, Neuron> &map,
                       const std::vector<Gene> genes) {

  double sum = 0;
  for (auto gene = incoming.begin(); gene != incoming.end(); ++gene) {
    const Gene &ref = genes[(*gene)];
    auto it = map.find(ref.into);
    if (it != map.end())
      sum += ref.weight * it->second.weight;
  }
  weight = sigmoid(sum);
}
void Neuron::add_gene(int32_t gene_number) { incoming.push_back(gene_number); }
void Neuron::set_weight(double weight_) { weight = weight_; }

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
