#pragma once

#include <cstdint>
#include <vector>
#include "genome.hpp"
#include "serial.hpp"
#define crossover_chance 0.75

using std::int32_t;
using std::vector;

struct Species : public Serial {
public:
  int32_t top_fitness;
  int32_t staleness;
  int32_t average_fitness;
  int32_t inputs;
  int32_t outputs;
  vector<Genome> genomes;

  bool save(ofstream &ofs) const;
  bool load(ifstream &ifs);
  virtual ~Species() {}

  void calculate_average_fitness();
  Genome breed_child();

  Species(int32_t inputs_, int32_t outputs_, Genome starter);
  Species(const Species &other);
  Species(Species &&other);
  Species &operator=(Species other);

  bool operator<(const Species &other) const;
  bool operator==(const Species &other) const;

private:
  void copy(const Species &other);
  void swap(Species &other);
};
