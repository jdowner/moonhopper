#include "TestDriver.h"
#include <iostream>

TestDriver::TestDriver() : m_tests(0)
{
}

void TestDriver::runTests()
{
  typedef std::vector<std::pair<std::string,TestFunction> > Tests;

  std::cout << "Starting tests" << std::endl;

  for(Tests::const_iterator it = m_tests.begin(); it != m_tests.end(); ++it)
  {
    if (not it->second())
    {
      std::cout << "Failed: " << it->first << std::endl;
    }
  }

  std::cout << "Tests complete" << std::endl;
}

void TestDriver::registerTest(const std::string& name, TestFunction test)
{
  m_tests.push_back(std::make_pair(name, test));
}
