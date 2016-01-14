#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "gene.hpp"
#include "serial.hpp"

using std::vector;
using std::int32_t;

struct Neuron : public Serial {
  vector<int32_t> incoming;
  double weight;

  double sigmoid(const double &x) const;
  void clear();

  bool save(ofstream &ofs) const;
  bool load(ifstream &ifs);

  Neuron() : weight(0) {}
  Neuron(const Neuron &other);
  Neuron(Neuron &&other);
  Neuron &operator=(Neuron other);
  virtual ~Neuron() {}

  void copy(const Neuron &here);
  void swap(Neuron &other);
};
