#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Genome
#include <boost/test/included/unit_test.hpp>
#include "genome.hpp"
#include "serial.hpp"

BOOST_AUTO_TEST_CASE(Genome_No_Arg_Contructor){
  int32_t inputs = 10, outputs = 4;
  Genome genome(inputs, outputs);
}

BOOST_AUTO_TEST_CASE(Genome_Reset_Neuron){
  int32_t inputs = 256, outputs = 10;
  Genome genome(inputs, outputs);
  for(size_t i = 0; i < 100; ++i){
    genome.generate_network();
  }
}
