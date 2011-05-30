#ifndef TESTDRIVER_H
#define TESTDRIVER_H

#include <vector>
#include <string>

typedef bool(*TestFunction)(void);
typedef std::vector<std::pair<std::string, TestFunction> > TestSuite;

/**
 *
 */
class TestDriver
{
  public:
    TestDriver();

    void runTests();
    void registerTest(const std::string& name, TestFunction test);
    void registerSuite(const TestSuite& suite);

  private:
    TestSuite m_tests;
};

#endif // TESTDRIVER_H

