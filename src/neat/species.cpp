#include "species.hpp"
#include <ctime>
#include <cstdlib>

Species::Species(int32_t inputs_, int32_t outputs_)
    : top_fitness(0), staleness(0), average_fitness(0), inputs(inputs_),
      outputs(outputs_) {}

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

  Genome child = genomes[static_cast<size_t>(rand() % genomes.size())];
  if (rand() / (double)RAND_MAX < crossover_chance) {
    auto &g1 = genomes[static_cast<size_t>(rand() % genomes.size())];
    auto &g2 = genomes[static_cast<size_t>(rand() % genomes.size())];
    child = g1.crossover(g2);
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
