#include "TestUtils.h"
#include <sstream>

std::pair<std::string,TestFunction> makeTest(
  const std::string& group,
  const std::string& name,
  TestFunction function)
{
  std::stringstream testname;
  testname << group << ":" << name;
  return std::make_pair(testname.str(),function);
}
