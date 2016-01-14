#include "pool.hpp"
#include <algorithm>
#include <tuple>
using std::tuple;
using std::get;

int32_t Pool::innovation = 0;

Pool::Pool(int32_t inputs_, int32_t outputs_)
    : generation(0), current_species(0), current_genome(0), current_frame(0),
      max_fitness(0), inputs(inputs_), outputs(outputs_) {
  // BI
  Species first_species(inputs_, outputs_);
  Genome first_genome(inputs_, outputs_);
  first_species.genomes.push_back(first_genome);
  species.push_back(first_species);
}
int32_t Pool::innovate() { return ++Pool::innovation; }

Pool::Pool(const Pool &other) { copy(other); }

Pool::Pool(Pool &&other) { swap(other); }

Pool &Pool::operator=(Pool other) {
  swap(other);
  return *this;
}

void Pool::copy(const Pool &other) {
  species = other.species;
  generation = other.generation;
  current_species = other.current_species;
  current_genome = other.current_genome;
  current_frame = other.current_frame;
  max_fitness = other.max_fitness;
  inputs = other.inputs;
  outputs = other.outputs;
}
void Pool::swap(Pool &other) {
  using std::swap;

  swap(species, other.species);
  swap(generation, other.generation);
  swap(current_species, other.current_species);
  swap(current_genome, other.current_genome);
  swap(current_frame, other.current_frame);
  swap(max_fitness, other.max_fitness);
  swap(inputs, other.inputs);
  swap(outputs, other.outputs);
}

void Pool::rank_globally() {
  /*You have to use pointers or else the original argument
          Will not be changed and you cannot use references in templates*/

  vector<tuple<int, int, int>> global;
  for (size_t i = 0; i != species.size(); ++i) {
    Species &s = species[i];
    for (size_t j = 0; j != s.genomes.size(); ++j) {
      global.push_back(tuple<int, int, int>(i, j, s.genomes[j].fitness));
    }
  }
  std::sort(global.begin(), global.end(),
            [](tuple<int, int, int> a, tuple<int, int, int> b) {
              return get<2>(a) < get<2>(b);
            });
  for (int32_t i = 0; i < global.size(); ++i) {
    auto tuple = global[i];
    species[get<0>(tuple)].genomes[get<1>(tuple)].global_rank = i + 1;
  }
}

int32_t Pool::calculate_average_fitness() {
  int32_t total = 0;
  for (auto s = species.begin(); s != species.end(); ++s) {
    total += s->average_fitness;
  }
  return total;
}

void Pool::cull_species(bool cut_to_one) {
  for (auto s = species.begin(); s != species.end(); ++s) {
    std::sort(s->genomes.begin(), s->genomes.end(), std::greater<Genome>());
    int32_t remaining = std::ceil(species.size() / 2);
    if (cut_to_one) {
      remaining = 1;
    }
    while (remaining-- > 0)
      s->genomes.pop_back();
  }
}

void Pool::remove_stale_species() {
  rank_globally();

  for (int32_t i = 0; i < species.size(); ++i) {
    Species s = species[i];
    std::sort(s.genomes.begin(), s.genomes.end(), std::greater<Genome>());
    s.staleness++;
    if (s.genomes[0].fitness > s.top_fitness) {
      s.top_fitness = s.genomes[0].fitness;
      s.staleness = 0;
    }
    if (s.staleness > stale_species && s.top_fitness <= max_fitness) {
      species.erase(
          std::remove(species.begin() + i, species.begin() + i + 1, s),
          species.end());
      --i;
    }
  }
}

void Pool::remove_weak_species() {

  auto sum = calculate_average_fitness();
  for (int32_t i = 0; i < species.size(); ++i) {
    Species &s = species[i];
    int32_t breed = int(s.average_fitness / sum * population);
    if (breed < 1) {
      species.erase(std::remove(species.begin(), species.end(), s),
                    species.end());
      --i;
    }
  }
}

void Pool::add_to_species(Genome child) {
  bool found_species = false;
  for (auto i = species.begin(); i != species.end() && !found_species; ++i) {
    if (child.same_species(i->genomes[0])) {
      i->genomes.push_back(child);
      found_species = true;
    }
  }

  if (!found_species) {
    Species childe(inputs, outputs);
    childe.genomes.push_back(child);
    species.push_back(childe);
  }
}

void Pool::new_generation() {
  cull_species(false);
  remove_stale_species();
  rank_globally();

  for (size_t s = 0; s != species.size(); ++s) {
    species[s].calculate_average_fitness();
  }

  remove_weak_species();
  int32_t sum = calculate_average_fitness();
  int added_species = 0;

  for (size_t j = 0; j != species.size(); ++j) {
    Species &s = species[j];
    s.calculate_average_fitness();
    int32_t breed = int(s.average_fitness / sum * population);
    for (int32_t i = 0; i < breed; ++i) {
      add_to_species(s.breed_child());
      added_species++;
    }
  }

  cull_species(true);
  int temp = population - static_cast<int>(species.size());
  if (temp < 0) {
    generation++;
    return;
  }
  while (added_species++ < temp) {
    add_to_species(species[rand() % (species.size() - 1)].breed_child());
  }

  generation++;
}

vector<bool> Pool::evaluate(const vector<int32_t> &inputs) {
  Species &current_spec = species[current_species];
  Genome &current_gen = current_spec.genomes[current_genome];
  current_gen.generate_network();

  return current_gen.evaluate(inputs);
}

void Pool::update_fitness(int32_t new_fitness) {
  top_genome().fitness = new_fitness;
  if (new_fitness > max_fitness) {
    max_fitness = new_fitness;
    ofstream stream("NEAT.dat");
    save(stream);
  }
  current_species = 0;
  current_genome = 0;
  while (fitness_measured()) {
    next_genome();
  }
}

bool Pool::save(ofstream &ofs) const {
  ofs << innovation << "\n" << generation << "\n" << current_species << "\n"
      << current_genome << "\n" << current_frame << "\n" << max_fitness << "\n"
      << inputs << "\n" << outputs << "\n";

  ofs << species.size() << "\n";
  for (auto i = species.begin(); i != species.end(); ++i) {
    i->save(ofs);
  }
  return true;
}

bool Pool::load(ifstream &ifs) {
  ifs >> innovation >> generation >> current_species >> current_genome >>
      current_frame >> max_fitness >> inputs >> outputs;
  size_t stuff;
  ifs >> stuff;
  species.clear();
  for (size_t i = 0; i < stuff; ++i) {
    Species temp(inputs, outputs);
    temp.load(ifs);
    species.push_back(temp);
  }
  return true;
}

void Pool::set_top() {
  int32_t maxfitness = 0, maxs, maxg;
  size_t s_ = 0, g_ = 0;
  for (auto s = species.begin(); s != species.end(); ++s, ++s_) {
    for (auto g = s->genomes.begin(); g != s->genomes.end(); ++g, ++g_) {
      if (g->fitness > maxfitness) {
        maxfitness = g->fitness;
        maxs = s_;
        maxg = g_;
      }
    }
  }

  current_species = maxs;
  current_genome = maxg;
  max_fitness = maxfitness;
  current_frame++;
}

void Pool::next_genome() {
  current_genome++;
  if (current_genome >= species[current_species].genomes.size()) {
    current_genome = 0;
    current_species++;
    if (current_species >= species.size()) {
      current_species = 0;
      new_generation();
    }
  }
}
bool Pool::fitness_measured() const {
  if (current_species >= species.size())
    return false;
  else if (current_genome >= species[current_species].genomes.size()) {
    return false;
  }
  return species[current_species].genomes[current_genome].fitness != 0;
}
