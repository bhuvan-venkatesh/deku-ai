#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <stdexcept>

#include "neuron.hpp"
#include "serial.hpp"
#include "testable.hpp"

#define max_nodes 10000
#define delta_disjoint 2.0
#define delta_weights 0.4
#define delta_threshold 1.0
#define stale_species 15

using std::unique_ptr;
using std::vector;
using std::int32_t;
using std::unordered_map;
using std::string;
using std::cout;

// This has the first input elements as the input neurons
// The next elements as intermediate nodes
// The final output elements as output nodes
typedef unordered_map<int32_t, Neuron> NetworkMap;
// In case there is a need to change later

template <typename T> T &random_element(vector<T> &elems) {
  if (elems.size() == 0)
    throw std::invalid_argument("No Elements");
  else if (elems.size() == 1)
    return elems[0];
  size_t index = rand() % (elems.size() - 1);
  return elems[index];
}

namespace action_chances {
const static float mutation_connect = (float)0.25, disturb = (float)0.9,
                   link = (float)2.0, node = (float)0.5,
                   bias_mutation = (float)0.4, step_size = (float)0.1,
                   disable_mutation = (float)0.4, enable_mutation = (float)0.2;
}

class Genome : public Serial
#if DEBUG
               ,
               public Testable
#endif
{
public:
  int32_t fitness;
  int32_t fitness_adjusted;
  int32_t max_neuron;
  int32_t global_rank;
  int32_t inputs;
  int32_t outputs;

  typedef struct {
    float connection = action_chances::mutation_connect;
    float disturb = action_chances::disturb;
    float link = action_chances::link;
    float bias = action_chances::bias_mutation;
    float node = action_chances::node;
    float enable = action_chances::enable_mutation;
    float disable = action_chances::disable_mutation;
    float step = action_chances::step_size;
  } chances;
  chances mutation_chance_rates;
  vector<Gene> genes;
  NetworkMap network;

  // void copy(Genome other);

  Genome(int32_t inputs_, int32_t outputs_);

  Genome(const Genome &other);
  Genome(Genome &&other);
  Genome &operator=(Genome other);

  void copy(const Genome &other);
  void swap(Genome &other);

  virtual ~Genome() {}

  bool operator==(const Genome &other) const;
  bool operator<(const Genome &other) const;
  bool operator>(const Genome &other) const;

  static Genome basic_genome(int32_t inputs, int32_t outputs);
  void mutate();
  void generate_network();
  vector<bool> evaluate(vector<int32_t> inputs);
  Genome crossover(const Genome &other) const;
  int32_t random_neuron(const bool &non_input) const;
  bool constains_link(const Gene &link) const;
  void point_mutate();
  void link_mutate(const bool &force_bias);
  void node_mutate();
  void toggle_enable(const bool &enabled);
  double disjoint(const Genome &other) const;
  double weights(const Genome &other) const;
  int call_times(float p);

  bool same_species(const Genome &other) const;

  bool save(ofstream &ofs) const;
  bool load(ifstream &ifs);

  void check_rep() const;

private:
  void reset_network_neurons();
  void connect_neurons();
  bool validate_input(const vector<int32_t> &inputs) const;
  void initialize_network_neuron(int32_t number);
  void update_network_weights(const vector<int32_t> &inputs);
  void evaluate_network();
  vector<bool> collect_button_commands();

  Genome random_gene_swap(const Genome &higher_fitness,
                          const Genome &lower_fitness) const;
  double rand_double();
  bool is_input_neuron(int32_t neuron_number) const;

  template <typename T> void mutate_rate(T &rate);
};
