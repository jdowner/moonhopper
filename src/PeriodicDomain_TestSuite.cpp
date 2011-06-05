#include "PeriodicDomain_TestSuite.h"
#include "PeriodicDomain.h"
#include "TestUtils.h"

namespace
{
  bool testConstruction()
  {
    PeriodicDomain domain(-1.0,0.0,-1.0,0.0);

    // Test the max/min accessors
    ASSERT_EQUAL(-1.0, domain.getMinX()); 
    ASSERT_EQUAL(0.0, domain.getMaxX()); 
    ASSERT_EQUAL(-1.0, domain.getMinY()); 
    ASSERT_EQUAL(0.0, domain.getMaxY()); 

    return true;
  }
}

bool testRemapping()
{
  PeriodicDomain domain(-1.0,0.0,-1.0,0.0);

  // Test the remapping functions
  ASSERT_EQUAL(-0.5, domain.toX(2.5));
  ASSERT_EQUAL(-0.5, domain.toX(-2.5));
  ASSERT_EQUAL(-0.5, domain.toY(2.5));
  ASSERT_EQUAL(-0.5, domain.toY(-2.5));

  return true;
}

TestSuite PeriodicDomain_TestSuite::getSuite()
{
  TestSuite suite;
  suite.push_back(TEST(PeriodicDomain,testConstruction));
  suite.push_back(TEST(PeriodicDomain,testRemapping));
  return suite;
}
