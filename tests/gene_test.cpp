#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Gene
#include <boost/test/included/unit_test.hpp>
#include "gene.hpp"
#include "serial.hpp"


BOOST_AUTO_TEST_CASE(Gene_Empty_Constructor){
	Gene gene;
  BOOST_CHECK_EQUAL(0, gene.into);
  BOOST_CHECK_EQUAL(0, gene.out);
  BOOST_CHECK_EQUAL(0, gene.weight);
  BOOST_CHECK_EQUAL(0, gene.innovation);
  BOOST_CHECK_EQUAL(true, gene.enabled);
}

BOOST_AUTO_TEST_CASE(Gene_Two_Constructor_Positive){
  Gene gene(1, 3);
  BOOST_CHECK_EQUAL(1, gene.into);
  BOOST_CHECK_EQUAL(3, gene.out);
  BOOST_CHECK_EQUAL(0, gene.weight);
  BOOST_CHECK_EQUAL(0, gene.innovation);
  BOOST_CHECK_EQUAL(true, gene.enabled);
}

BOOST_AUTO_TEST_CASE(Gene_Two_Constructor_Negative){
  Gene gene(-1, -3);
  BOOST_CHECK_EQUAL(-1, gene.into);
  BOOST_CHECK_EQUAL(-3, gene.out);
  BOOST_CHECK_EQUAL(0, gene.weight);
  BOOST_CHECK_EQUAL(0, gene.innovation);
  BOOST_CHECK_EQUAL(true, gene.enabled);
}

BOOST_AUTO_TEST_CASE(Gene_Access){
  Gene gene;
  gene.into = 1;
  gene.out = 2;
  gene.weight = 3;
  gene.innovation = 4;
  gene.enabled = false;

  BOOST_CHECK_EQUAL(1, gene.into);
  BOOST_CHECK_EQUAL(2, gene.out);
  BOOST_CHECK_EQUAL(3, gene.weight);
  BOOST_CHECK_EQUAL(4, gene.innovation);
  BOOST_CHECK_EQUAL(false, gene.enabled);
}

BOOST_AUTO_TEST_CASE(Gene_Operator_Less){
  Gene lhs(0, 100);
  Gene rhs(0, 101);
  Gene mhs(0, 100);

  BOOST_CHECK_EQUAL(true, lhs < rhs);
  BOOST_CHECK_EQUAL(false, rhs < lhs);
  BOOST_CHECK_EQUAL(false, lhs < mhs);
}

BOOST_AUTO_TEST_CASE(Gene_Operator_Equals){
  Gene lhs;
  lhs.innovation = 1;

  Gene rhs;
  rhs.innovation = 2;

  Gene mhs;
  mhs.innovation = 1;


  BOOST_CHECK_EQUAL(true, lhs == mhs);
  BOOST_CHECK_EQUAL(false, rhs == lhs);
}

BOOST_AUTO_TEST_CASE(Gene_Save_Load){
	Gene gene;
  gene.into = 1;
  gene.out = 2;
  gene.weight = 3;
  gene.innovation = 4;
  gene.enabled = false;

	ofstream test("gene_test.txt");
	gene.save(test);

	test.close();

	Gene temp;
	ifstream load("gene_test.txt");
	temp.load(load);

	BOOST_CHECK_EQUAL(temp.into, gene.into);
	BOOST_CHECK_EQUAL(temp.out, gene.out);
	BOOST_CHECK_EQUAL(temp.weight, gene.weight);
	BOOST_CHECK_EQUAL(temp.innovation, gene.innovation);
	BOOST_CHECK_EQUAL(temp.enabled, gene.enabled);

}
