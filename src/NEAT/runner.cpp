/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_Gene_Tester_init = false;
#include "/home/bhuvan/Projects/ai_game/src/NEAT/gene_test.cpp"

static Gene_Tester suite_Gene_Tester;

static CxxTest::List Tests_Gene_Tester = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_Gene_Tester( "gene_test.cpp", 4, "Gene_Tester", suite_Gene_Tester, Tests_Gene_Tester );

static class TestDescription_suite_Gene_Tester_test_Gene_Constructor_Empty : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_Gene_Tester_test_Gene_Constructor_Empty() : CxxTest::RealTestDescription( Tests_Gene_Tester, suiteDescription_Gene_Tester, 7, "test_Gene_Constructor_Empty" ) {}
 void runTest() { suite_Gene_Tester.test_Gene_Constructor_Empty(); }
} testDescription_suite_Gene_Tester_test_Gene_Constructor_Empty;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
