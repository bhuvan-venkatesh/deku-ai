#include "species.hpp"
#include <ctime>
#include <cstdlib>
#include <algorithm>

Species::Species(int32_t inputs_, int32_t outputs_, Genome starter)
    : top_fitness(0), staleness(0), average_fitness(0), inputs(inputs_),
      outputs(outputs_) {
  genomes.push_back(starter);
}

Species::Species(const Species &other) { copy(other); }

Species::Species(Species &&other)
    : Species(other.inputs, other.outputs, other.genomes[0]) {
  swap(other);
}

Species &Species::operator=(Species other) {
  swap(other);
  return *this;
}

void Species::copy(const Species &other) {
  top_fitness = other.top_fitness;
  staleness = other.staleness;
  average_fitness = other.average_fitness;
  inputs = other.inputs;
  outputs = other.outputs;
  genomes = other.genomes;
}

void Species::swap(Species &other) {
  using std::swap;

  swap(top_fitness, other.top_fitness);
  swap(staleness, other.staleness);
  swap(average_fitness, other.average_fitness);
  swap(inputs, other.inputs);
  swap(outputs, other.outputs);
  swap(genomes, other.genomes);
}

bool Species::operator<(const Species &other) const {
  return average_fitness < other.average_fitness;
}

bool Species::operator==(const Species &other) const {
  return top_fitness == other.top_fitness && staleness == other.staleness &&
         average_fitness == other.average_fitness && genomes == other.genomes;
}

void Species::calculate_average_fitness() {
  int32_t total = 0;
  for (auto g = genomes.begin(); g != genomes.end(); ++g) {
    total += g->global_rank;
  }
  average_fitness = total / double(genomes.size());
}

Genome Species::breed_child() {
  srand(time(NULL));

  if (genomes.size() == 0) {
    Genome not_temp(inputs, outputs);
    return not_temp;
  }

  Genome child = random_element(genomes);
  if (rand() / (double)RAND_MAX < crossover_chance) {
    Genome g1 = random_element(genomes);
    Genome g2 = random_element(genomes);
    Genome scoped = g1.crossover(g2);
    child = scoped;
  }

  child.mutate();
  return child;
}

bool Species::save(ofstream &ofs) const {
  ofs << top_fitness << "\n";
  ofs << staleness << "\n" << average_fitness << "\n" << inputs << "\n"
      << outputs << "\n";
  ofs << genomes.size() << "\n";
  for (auto i = genomes.begin(); i != genomes.end(); ++i) {
    i->save(ofs);
  }
  return true;
}

bool Species::load(ifstream &ifs) {
  ifs >> top_fitness >> staleness >> average_fitness >> inputs >> outputs;
  genomes.clear();
  size_t genomes_size;
  ifs >> genomes_size;
  for (size_t i = 0; i < genomes_size; ++i) {
    Genome temp(inputs, outputs);
    temp.load(ifs);
    genomes.push_back(temp);
  }
  return true;
}
