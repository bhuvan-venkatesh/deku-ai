#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Genome
#include <boost/test/included/unit_test.hpp>
#include "genome.hpp"
#include "serial.hpp"

struct Massive {
  int m;

  Massive() : m(2) { BOOST_TEST_MESSAGE("setup mass"); }

  ~Massive() { BOOST_TEST_MESSAGE("teardown mass"); }
};
/*
Genome(int32_t inputs_, int32_t outputs_);
Genome(const Genome &other);
Genome(Genome &&other);
Genome &operator=(Genome other);
virtual ~Genome(){};

bool operator==(const Genome &other) const;
bool operator<(const Genome &other) const;
bool operator>(const Genome &other) const;

void generate_network();
vector<bool> evaluate(vector<int32_t> inputs);
void mutate();
Genome crossover(const Genome &other) const;
bool same_species(const Genome &other) const;

bool save(ofstream &ofs) const;
bool load(ifstream &ifs); */

BOOST_FIXTURE_TEST_SUITE(Physics, Massive)

BOOST_AUTO_TEST_CASE(Genome_No_Arg_Contructor) {
  int32_t inputs = 10, outputs = 4;
  Genome genome(inputs, outputs);
}

BOOST_AUTO_TEST_CASE(Genome_Reset_Neuron) {
  int32_t inputs = 256, outputs = 10;
  Genome genome(inputs, outputs);
  for (size_t i = 0; i < 100; ++i) {
    genome.generate_network();
  }
}
