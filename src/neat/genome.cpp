#include "genome.hpp"
#include "pool.hpp"
#include <algorithm>
#include <iostream>
#include <cstdlib> /* srand, rand */
#include <ctime>
using std::cout;

Genome::Genome(int32_t inputs_, int32_t outputs_)
    : fitness(0), fitness_adjusted(0), max_neuron(0), global_rank(0),
      inputs(inputs_), outputs(outputs_) {
  // generate_network();
  if (inputs_ < 1 || outputs_ < 1) {
    throw std::invalid_argument("received negative value");
  }
}

void Genome::copy(const Genome &other) {
  fitness = other.fitness;
  fitness_adjusted = other.fitness_adjusted;
  max_neuron = other.max_neuron;
  global_rank = other.global_rank;
  inputs = other.inputs;
  outputs = other.outputs;

  mutation_chance_rates.connection = other.mutation_chance_rates.connection;
  mutation_chance_rates.disturb = other.mutation_chance_rates.disturb;
  mutation_chance_rates.link = other.mutation_chance_rates.link;
  mutation_chance_rates.bias = other.mutation_chance_rates.bias;
  mutation_chance_rates.node = other.mutation_chance_rates.node;
  mutation_chance_rates.enable = other.mutation_chance_rates.enable;
  mutation_chance_rates.disable = other.mutation_chance_rates.disable;
  mutation_chance_rates.step = other.mutation_chance_rates.step;

  genes = vector<Gene>(other.genes);
  network = NetworkMap(other.network);
}

void Genome::swap(Genome &other) {
  using std::swap;
  swap(fitness, other.fitness);
  swap(fitness_adjusted, other.fitness_adjusted);
  swap(max_neuron, other.max_neuron);
  swap(global_rank, other.global_rank);
  swap(inputs, other.inputs);
  swap(outputs, other.outputs);

  swap(mutation_chance_rates.connection,
       other.mutation_chance_rates.connection);
  swap(mutation_chance_rates.disturb, other.mutation_chance_rates.disturb);
  swap(mutation_chance_rates.link, other.mutation_chance_rates.link);
  swap(mutation_chance_rates.bias, other.mutation_chance_rates.bias);
  swap(mutation_chance_rates.node, other.mutation_chance_rates.node);
  swap(mutation_chance_rates.enable, other.mutation_chance_rates.enable);
  swap(mutation_chance_rates.disable, other.mutation_chance_rates.disable);
  swap(mutation_chance_rates.step, other.mutation_chance_rates.step);

  swap(genes, other.genes);
  swap(network, other.network);
}

Genome::Genome(const Genome &other) { copy(other); }

Genome &Genome::operator=(Genome other) {
  swap(other);
  return *this;
}

Genome::Genome(Genome &&other) { swap(other); }

bool Genome::operator==(const Genome &other) const {
  return fitness == other.fitness &&
         fitness_adjusted == other.fitness_adjusted &&
         max_neuron == other.max_neuron && global_rank == other.global_rank &&
         inputs == other.inputs && outputs == other.outputs;
}

bool Genome::operator<(const Genome &other) const {
  return fitness < other.fitness;
}

bool Genome::operator>(const Genome &other) const {
  return fitness > other.fitness;
}

Genome Genome::basic_genome(int32_t inputs, int32_t outputs) {
  Genome return_value(inputs, outputs);

  return_value.max_neuron = inputs - 1;
  return_value.mutate();

  return return_value;
}

void Genome::mutate() {
  // TODO: Generated stub, find an appropriate
  // Mutation algorithm
  mutate_rate(mutation_chance_rates.connection);
  mutate_rate(mutation_chance_rates.disturb);
  mutate_rate(mutation_chance_rates.link);
  mutate_rate(mutation_chance_rates.bias);
  mutate_rate(mutation_chance_rates.node);
  mutate_rate(mutation_chance_rates.enable);
  mutate_rate(mutation_chance_rates.disable);
  mutate_rate(mutation_chance_rates.step);

  if (rand_double() < mutation_chance_rates.connection)
    point_mutate();

  for (int i = call_times(mutation_chance_rates.link); i > 0; --i)
    link_mutate(false);
  for (int i = call_times(mutation_chance_rates.bias); i > 0; --i)
    link_mutate(true);
  for (int i = call_times(mutation_chance_rates.node); i > 0; --i)
    node_mutate();
  for (int i = call_times(mutation_chance_rates.enable); i > 0; --i)
    toggle_enable(true);
  for (int i = call_times(mutation_chance_rates.disable); i > 0; --i)
    toggle_enable(false);
}

int Genome::call_times(float p) {
  int times = 0;
  for (; p > 0; --p) {
    if (rand_double() < p)
      times++;
  }
  return times;
}

void Genome::generate_network() {
  reset_network_neurons();
  connect_neurons();
}

vector<bool> Genome::evaluate(vector<int32_t> input_) {
  if (!validate_input(input_)) {
    return vector<bool>();
  }
  update_network_weights(input_);
  return collect_button_commands();
}

Genome Genome::crossover(const Genome &other) const {
  Genome child(inputs, outputs);
  if (this->fitness > other.fitness) {
    child = random_gene_swap(*this, other);
    child.mutation_chance_rates = this->mutation_chance_rates;
  } else {
    child = random_gene_swap(other, *this);
    child.mutation_chance_rates = other.mutation_chance_rates;
  }
  child.max_neuron =
      max_neuron > other.max_neuron ? max_neuron : other.max_neuron;

  return child;
}

int32_t Genome::random_neuron(const bool &non_input) const {
  vector<int32_t> neurons;
  for (int32_t i = 0; !non_input && i < inputs; ++i) {
    neurons.push_back(i);
  }
  for (int32_t i = 0; i < outputs; ++i) {
    neurons.push_back(max_nodes + i);
  }
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    if (!non_input || !is_input_neuron(i->into)) {
      neurons.push_back(i->into);
    }
    if (!non_input || !is_input_neuron(i->out)) {
      neurons.push_back(i->out);
    }
  }
  srand((unsigned int)time(NULL));
  return random_element(neurons);
}

bool Genome::constains_link(const Gene &link) const {
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    if (i->same_link(link)) {
      return true;
    }
  }
  return false;
}

double Genome::rand_double() { return double(rand()) / RAND_MAX; }

void Genome::point_mutate() {
  srand((unsigned int)time(NULL));
  const float &step = mutation_chance_rates.step;
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    if (rand_double() < mutation_chance_rates.disturb)
      i->weight = i->weight + rand_double() * step * 2 - (double)step;
    else
      i->weight = (rand_double() * 4.0 - 2.0);
  }
}

void Genome::link_mutate(const bool &force_bias) {
  int32_t neuron1 = random_neuron(false), neuron2 = random_neuron(true);
  if (is_input_neuron(neuron1) && is_input_neuron(neuron2))
    return;
  if (is_input_neuron(neuron2))
    std::swap(neuron1, neuron2);
  Gene link(neuron1, neuron2);
  if (force_bias) {
    link.into = inputs - 1;
  }
  if (constains_link(link)) {
    return;
  }
  link.innovation = Pool::innovate();
  link.weight = (rand_double() * 4 - 2);

  genes.push_back(link);
}

void Genome::node_mutate() {
  if (genes.size() == 0)
    return;
  max_neuron++;
  Gene &gene = random_element(genes);
  if (!gene.enabled)
    return;
  gene.enabled = false;

  Gene inp1(gene);
  inp1.out = (max_neuron);
  inp1.weight = (1.0);
  inp1.innovation = (Pool::innovate());
  inp1.enabled = true;

  Gene gene2(gene);
  gene2.into = (max_neuron);
  gene2.innovation = (Pool::innovate());
  gene2.enabled = true;

  genes.push_back(gene2);
  genes.push_back(inp1);
}

void Genome::toggle_enable(const bool &enable) {
  vector<size_t> candidates;
  for (size_t i = 0; i != genes.size(); ++i) {
    if (genes[i].enabled == !enable) {
      candidates.push_back(i);
    }
  }
  if (candidates.size() == 0)
    return;
  auto &random_candidate = genes[random_element(candidates)];
  random_candidate.enabled = (!random_candidate.enabled);
}

// TODO: make more efficient if need be
double Genome::disjoint(const Genome &other) const {
  int32_t disjoint_genes = 0;
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    if (std::find(other.genes.begin(), other.genes.end(), *i) ==
        other.genes.end()) {
      disjoint_genes++;
    }
  }
  for (auto i = other.genes.begin(); i != other.genes.end(); ++i) {
    if (std::find(genes.begin(), genes.end(), *i) == genes.end()) {
      disjoint_genes++;
    }
  }

  return double(disjoint_genes) / std::max(genes.size(), other.genes.size());
}

double Genome::weights(const Genome &other) const {
  double sum = 0;
  int32_t incident = 0;
  for (auto i = other.genes.begin(); i != other.genes.end(); ++i) {
    auto gene2 = std::find(genes.begin(), genes.end(), *i);
    if (gene2 != genes.end()) {
      sum += std::abs(i->weight - gene2->weight);
      incident++;
    }
  }
  return sum / incident;
}

bool Genome::same_species(const Genome &other) const {
  double delta_d = delta_disjoint * disjoint(other);
  double delta_w = delta_weights * weights(other);
  return delta_d + delta_w < delta_threshold;
}

bool Genome::save(ofstream &ofs) const {
  ofs << fitness << "\n";
  ofs << fitness_adjusted << "\n";
  ofs << max_neuron << "\n";
  ofs << global_rank << "\n";
  ofs << inputs << "\n";
  ofs << outputs << "\n";

  ofs << mutation_chance_rates.connection << "\n";
  ofs << mutation_chance_rates.disturb << "\n";
  ofs << mutation_chance_rates.link << "\n";
  ofs << mutation_chance_rates.bias << "\n";
  ofs << mutation_chance_rates.node << "\n";
  ofs << mutation_chance_rates.enable << "\n";
  ofs << mutation_chance_rates.disable << "\n";
  ofs << mutation_chance_rates.step << "\n";

  ofs << network.size() << "\n";
  for (auto i = network.begin(); i != network.end(); ++i) {
    ofs << i->first << "\n";
    i->second.save(ofs);
  }

  ofs << genes.size() << "\n";
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    i->save(ofs);
  }

  return true;
}

bool Genome::load(ifstream &ifs) {
  ifs >> fitness >> fitness_adjusted >> max_neuron >> global_rank >> inputs >>
      outputs;

  ifs >> mutation_chance_rates.connection >> mutation_chance_rates.disturb >>
      mutation_chance_rates.link >> mutation_chance_rates.bias >>
      mutation_chance_rates.node >> mutation_chance_rates.enable >>
      mutation_chance_rates.disable >> mutation_chance_rates.step;

  size_t network_size;
  ifs >> network_size;
  for (size_t i = 0; i < network_size; ++i) {
    Neuron temp;
    int32_t position;
    ifs >> position;
    temp.load(ifs);

    // We are done deserializing one neuron and linking properly
    network[position] = temp;
  }

  size_t genes_size;
  ifs >> genes_size;
  for (size_t i = 0; i < genes_size; ++i) {
    Gene temp;
    temp.load(ifs);
    genes.push_back(temp);
  }

  return true;
}

/***************************************************






                        Private Member Functions







****************************************************/

void Genome::reset_network_neurons() {

  for (int32_t i = 0; i < inputs; ++i) {
    network[static_cast<size_t>(i)].clear();
  }
  for (int32_t i = 0; i < outputs; ++i) {
    network[static_cast<size_t>(i + max_nodes)].clear();
  }
}

void Genome::connect_neurons() {
  for (size_t j = 0; j != genes.size(); ++j) {
    Gene &i = genes[j];
    if (i.enabled) {
      initialize_network_neuron(i.out);
      network[i.out].incoming.push_back(j);
      initialize_network_neuron(i.into);
    }
  }
}

void Genome::initialize_network_neuron(int32_t number) {
  if (network.find(number) == network.end()) {
    network[number].clear();
  }
}

bool Genome::validate_input(const vector<int32_t> &inputs_) const {
  if (inputs_.size() != inputs) {
    std::cout << "Incorrect number of inputs to the network";
    return false;
  }
  return true;
}

void Genome::update_network_weights(const vector<int32_t> &inputs) {
  check_rep();
  for (int32_t i = 0; i < this->inputs; ++i) {
    network[i].weight = inputs[i];
  }
}

void Genome::evaluate_network() {

  for (auto &pair : network) {
    Neuron &current = pair.second;

    auto &vec_ptr = current.incoming;
    double sum = 0;
    for (auto incoming_gene = vec_ptr.begin(); incoming_gene != vec_ptr.end();
         ++incoming_gene) {
      Gene &ref = genes[(*incoming_gene)];
      sum += ref.weight * network[ref.into].weight;
    }
    current.weight = current.sigmoid(sum);
  }
}

vector<bool> Genome::collect_button_commands() {
  vector<bool> output_return;
  for (int32_t i = 0; i < outputs; ++i) {
    // > 0 is the test for whether to push button or not
    output_return.push_back(network[max_nodes + i].weight > 0);
  }
  return output_return;
}

Genome Genome::random_gene_swap(const Genome &higher_fitness,
                                const Genome &lower_fitness) const {
  Genome child(inputs, outputs);
  std::unordered_map<int32_t, Gene> innov2;
  for (auto i = lower_fitness.genes.begin(); i != lower_fitness.genes.end();
       ++i) {
    innov2[i->innovation] = *i;
  }
  srand(time(NULL));
  for (auto i = higher_fitness.genes.begin(); i != higher_fitness.genes.end();
       ++i) {
    Gene gene1 = Gene(*i);
    if (innov2.find(gene1.innovation) != innov2.end()) {
      Gene gene2 = Gene(innov2[gene1.innovation]);
      if (rand() % 2 && gene2.enabled) {
        child.genes.push_back(gene2);
        continue;
      }
    }
    child.genes.push_back(gene1);
  }

  return child;
}

bool Genome::is_input_neuron(int32_t neuron_number) const {
  return neuron_number < inputs;
}

template <typename T> void Genome::mutate_rate(T &rate) {
  rate = (.95 + rand() % 2 * .10263) * rate;
}

void Genome::check_rep() const {
  cout << "fitness:\t\t" << fitness << "\n"
       << "fitness adjusted:\t" << fitness_adjusted << "\n"
       << "max neuron:\t\t" << max_neuron << "\n"
       << "global rank:\t\t" << global_rank << "\n"
       << "inputs:\t\t\t" << inputs << "\n"
       << "outputs:\t\t" << outputs << "\n";

  cout << "connection:\t\t" << mutation_chance_rates.connection << "\n"
       << "disturb:\t\t" << mutation_chance_rates.disturb << "\n"
       << "link:\t\t\t" << mutation_chance_rates.link << "\n"
       << "bias:\t\t\t" << mutation_chance_rates.bias << "\n"
       << "node:\t\t\t" << mutation_chance_rates.node << "\n"
       << "enable:\t\t\t" << mutation_chance_rates.enable << "\n"
       << "disable:\t\t" << mutation_chance_rates.disable << "\n"
       << "step:\t\t\t" << mutation_chance_rates.step << "\n";

  cout << "network size:\t\t" << network.size() << "\n";

  cout << "gene size:\t\t" << genes.size() << "\n";
  /*
  for (auto i = genes.begin(); i != genes.end(); ++i) {
    i->save(ofs);
  }*/
  cout << endl;
}
