#ifndef TESTDRIVER_H
#define TESTDRIVER_H

#include <vector>
#include <string>

typedef bool(*TestFunction)(void);

/**
 *
 */
class TestDriver
{
  public:
    TestDriver();

    void runTests();
    void registerTest(const std::string& name, TestFunction test);

  private:
    std::vector<std::pair<std::string, TestFunction> > m_tests;
};

#endif // TESTDRIVER_H

