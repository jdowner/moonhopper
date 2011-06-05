#include "Animation_TestSuite.h"
#include "Animation.h"
#include "TestUtils.h"

namespace
{
  bool testConstruction()
  {
    return false;
  }
}

TestSuite Animation_TestSuite::getSuite()
{
  TestSuite suite;
  suite.push_back(TEST(Animation,testConstruction));
  return suite;
}
