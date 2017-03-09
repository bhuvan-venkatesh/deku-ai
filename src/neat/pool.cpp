#include "pool.hpp" //
#include <algorithm>
#include <tuple>
using std::tuple;
using std::get;

int32_t Pool::innovation = 0;

Pool::Pool(int32_t inputs_, int32_t outputs_)
    : generation(0), current_species(0), current_genome(0), current_frame(0),
      max_fitness(0), inputs(inputs_), outputs(outputs_) {
  /*Constructor for the pool of algorithms that utilizes a 32-bit signed integer inputs_ as bounded input.
  At the start of the pool, there is only one genetic algorithm (the common ancestor) which is randomly generated.*/
  Genome first_genome(inputs_, outputs_);
  Species first_species(inputs_, outputs_, first_genome);
  species.push_back(first_species);
}
int32_t Pool::innovate() { return ++Pool::innovation; }//Updates the evolutionary phase count.

Pool::Pool(const Pool &other) { copy(other); }//Alternate constructor for preserving a desired pool.

Pool::Pool(Pool &&other) { swap(other); }//Alternate constructure used for arranging the pools in order of most desired.

Pool &Pool::operator=(Pool other) {//The created operator used for switching pools.
  swap(other);
  return *this;
}

void Pool::copy(const Pool &other) {
  //Attributes from one pool can be replicated to another pool.
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
  //Using pool's swaping operator attributes between 2 pools can be swapped around.
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
  /*Dictates the best possible pool in order to envoke evolution*/
  vector<tuple<int, int, int>> global;
  //Iterates through every species in the pool and places them into a roster tuple called global.
  for (size_t i = 0; i != species.size(); ++i) {
    //s: pointer to an individual species in global.
    Species &s = species[i];
    for (size_t j = 0; j != s.genomes.size(); ++j) {
      global.push_back(tuple<int, int, int>(i, j, s.genomes[j].fitness));
    }
  }
  //std::sort(...): Compares two species to see who has a better fitness.
  std::sort(global.begin(), global.end(),
            [](tuple<int, int, int> a, tuple<int, int, int> b) {
              return get<2>(a) < get<2>(b);
            });
  //Iterates from highest to lowest rank in the tuple global, and provides a corresponding number for ranking.
  for (int32_t i = 0; i < global.size(); ++i) {
    auto tuple = global[i];
    species[get<0>(tuple)].genomes[get<1>(tuple)].global_rank = i + 1;
  }
}

int32_t Pool::calculate_average_fitness() {
  //Takes the sum of all of the fitnesses and divides that value by the number of species, which is then returned.
  //total: the sum of all of the species' fintess values.
  int32_t total = 0;
  //Adds the fitness values of all the species in the pool.
  for (auto s = species.begin(); s != species.end(); ++s) {
    total += s->average_fitness;
  }
  return total;
}

void Pool::cull_species(bool cut_to_one) {
  //Removes all of the species with too low of a fitness
  //cut_to_one: bool that dictates whether only the top ranked should remain.
  for (auto s = species.begin(); s != species.end(); ++s) {
    std::sort(s->genomes.begin(), s->genomes.end(), std::greater<Genome>());
    int32_t remaining = std::ceil(species.size() / 2);
    if (cut_to_one) {
      remaining = 1;
    }
    while (remaining-- > 0 && s->genomes.size() > 1)
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
    Species childe(inputs, outputs, child);
    species.push_back(childe);
  }
}

void Pool::new_generation() {
  std::cout << "1" << std::endl;
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
    species[j].calculate_average_fitness();
    int32_t breed = int(species[j].average_fitness / sum * population);
    for (int32_t i = 0; i < breed; ++i) {
      add_to_species(species[j].breed_child());
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
    add_to_species(random_element(species).breed_child());
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
    ofstream stream("NEAT.dat", std::ofstream::trunc);
    save(stream);
  }
  current_species = 0;
  current_genome = 0;
  while (fitness_measured()) {
    next_genome();
  }
}

void Pool::generate_top_network() { top_genome().generate_network(); } //Produces a network of the top genomes to enforce survival of the fittest.

Genome &Pool::top_genome() {
  return species[static_cast<size_t>(current_species)]
      .genomes[static_cast<size_t>(current_genome)];
}

void Pool::reset_frame() { current_frame = 0; }

void Pool::advance_frame() { ++current_frame; }

int32_t Pool::get_frame() { return current_frame; }

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
  //Inputs for a generation are extracted from an input file (recovers previous progress before the last termination)
  ifs >> innovation >> generation >> current_species >> current_genome >>
      current_frame >> max_fitness >> inputs >> outputs;
  size_t stuff;
  ifs >> stuff;
  species.clear();
  for (size_t i = 0; i < stuff; ++i) {
    Species temp(inputs, outputs, Genome(inputs, outputs));
    temp.load(ifs);
    if (temp.genomes.size() > 1)
      temp.genomes.erase(std::remove(temp.genomes.begin(),
                                     temp.genomes.begin() + 1,
                                     Genome(inputs, outputs)),
                         temp.genomes.end());
    species.push_back(temp);
  }
  return true;
}

void Pool::set_top() {
  //Determines the species and its genome with the greatest fitness.
  int32_t maxfitness = 0, maxs, maxg;
  size_t s_ = 0, g_ = 0;
  //Scans through each of the species in the generation.
  for (auto s = species.begin(); s != species.end(); ++s, ++s_) {
    //Scans through each genome in a particular species
    for (auto g = s->genomes.begin(); g != s->genomes.end(); ++g, ++g_) {
      //Checks to see if there is a record fitness is exceeded, which is then updated.
      if (g->fitness > maxfitness) {
        maxfitness = g->fitness;
        maxs = s_;
        maxg = g_;
      }
    }
  }
  //The species containing the genome of highest fitness is then naturally selected.
  current_species = maxs;
  current_genome = maxg;
  max_fitness = maxfitness;
  current_frame++;
}

void Pool::next_genome() {
  //A new generation is created if the number of species in the current pool is exceeded.
  if (current_species >= species.size()) {
    current_genome = 0;
    current_species = 0;
    new_generation();
  }
  //Incrementation by one equates to viewing the next genome.
  current_genome++;
  //A new species is created if the current genome number exceeds the size of a species's genome.
  if (current_genome >= species[current_species].genomes.size()) {
    current_genome = 0;
    current_species++;
    //A new generation is created if the number of species in the current pool is exceeded.
    if (current_species >= species.size()) {
      current_species = 0;
      new_generation();
    }
  }
}

bool Pool::fitness_measured() const {
  //Pool:fitness_measured() : checks to see whether the species fitness was measured.
  //if the species lands outside of the size of the species list, fitness has not been measured.
  if (current_species >= species.size())
    return false;
  //if the genome lands outside of the size of the genome list, fitness has not been measured.
  else if (current_genome >= species[current_species].genomes.size()) {
    return false;
  }
  return species[current_species].genomes[current_genome].fitness != 0;
}
