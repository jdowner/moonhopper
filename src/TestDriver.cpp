#include "TestDriver.h"
#include <iostream>
#include "PeriodicDomain_TestSuite.h"
#include "Animation_TestSuite.h"

TestDriver::TestDriver() : m_tests(0)
{
  registerSuite(PeriodicDomain_TestSuite::getSuite());
  registerSuite(Animation_TestSuite::getSuite());
}

void TestDriver::runTests()
{
  typedef std::vector<std::pair<std::string,TestFunction> > Tests;

  std::cout << "Starting tests" << std::endl;

  for(Tests::const_iterator it = m_tests.begin(); it != m_tests.end(); ++it)
  {
    try
    {
      if (not it->second())
      {
        std::cout << "Failure: " << it->first << std::endl;
      }
      else
      {
        std::cout << "Success: " << it->first << std::endl;
      }
    }
    catch (std::exception& e)
    {
      std::cout << "Error: " << e.what() << std::endl;
    }
    catch (...)
    {
      std::cout << "Error: Unrecognized exception" << std::endl;
    }
  }

  std::cout << "Tests complete" << std::endl;
}

void TestDriver::registerTest(const std::string& name, TestFunction test)
{
  m_tests.push_back(std::make_pair(name, test));
}

void TestDriver::registerSuite(const TestSuite& suite)
{
  m_tests.insert(m_tests.begin(), suite.begin(), suite.end());
}
