#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include "TestDriver.h"

std::pair<std::string,TestFunction> makeTest(
  const std::string& group,
  const std::string& name,
  TestFunction function);

#define TEST(Group,Function) makeTest(#Group,#Function,&Function)
#define ASSERT_EQUAL(expected,actual) if ((expected) != actual) return false;

#endif // TESTUTILS_H

