#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include <iostream>
#include "TestDriver.h"

std::pair<std::string,TestFunction> makeTest(
  const std::string& group,
  const std::string& name,
  TestFunction function);

template <typename S, typename T>
bool test_equal(const S& expected, const T& actual)
{
  if (expected != actual)
  {
    std::cerr << "Failure: " << expected << " != " << actual << std::endl;
    return false;
  }

  return true;
}

#define TEST(Group,Function) makeTest(#Group,#Function,&Function)
#define ASSERT_EQUAL(expected,actual) \
  { if (!test_equal((expected),(actual))) return false; }

#endif // TESTUTILS_H

