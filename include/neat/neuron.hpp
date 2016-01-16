#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "gene.hpp"
#include "serial.hpp"

using std::vector;
using std::int32_t;

struct Neuron : public Serial {
public:
  Neuron() : weight(0) {}
  Neuron(const Neuron &other);
  Neuron(Neuron &&other);
  Neuron &operator=(Neuron other);
  virtual ~Neuron() {}

  double sigmoid(const double &x) const;

  bool save(ofstream &ofs) const;
  bool load(ifstream &ifs);

  void propogate(const std::unordered_map<int32_t, Neuron> &map,
                 const std::vector<Gene> genes);
  bool evaluate() const;

  void set_weight(double weight_);
  void clear();
  void add_gene(int32_t gene_number);

private:
  void copy(const Neuron &here);
  void swap(Neuron &other);

  vector<int32_t> incoming;
  double weight;
};
