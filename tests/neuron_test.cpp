#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Gene
#include <boost/test/included/unit_test.hpp>
#include "neuron.hpp"
#include "serial.hpp"

BOOST_AUTO_TEST_CASE(Neuron_Sigmod){
  Neuron neuron;
  BOOST_CHECK_CLOSE(-.985216, neuron.sigmoid(-1), .01);
  BOOST_CHECK_CLOSE(0, neuron.sigmoid(0), .01);
  BOOST_CHECK_CLOSE(.985216, neuron.sigmoid(1), .01);
}

BOOST_AUTO_TEST_CASE(Neuron_Clear){
  Neuron neuron;
  neuron.weight = 10;
  for(int i =0; i < 10; ++i)
    neuron.incoming.push_back(32);
  neuron.clear();
  BOOST_CHECK_EQUAL(0, neuron.weight);
  BOOST_CHECK_EQUAL((size_t)0, neuron.incoming.size());
}

BOOST_AUTO_TEST_CASE(Neuron_Save_Load){
  Neuron neuron;
  neuron.weight = 10;
  for(int i =0; i < 10; ++i)
    neuron.incoming.push_back(32);
  ofstream file("neuron_test.txt");
  neuron.save(file);
  file.close();

  Neuron other;
  ifstream data("neuron_test.txt");
  other.load(data);
  data.close();

  BOOST_CHECK_EQUAL(neuron.weight, other.weight);
  BOOST_CHECK_EQUAL(neuron.incoming.size(), other.incoming.size());
  for(size_t i = 0; i < neuron.incoming.size(); ++i)
    BOOST_CHECK_EQUAL(neuron.incoming[i], other.incoming[i]);
}
