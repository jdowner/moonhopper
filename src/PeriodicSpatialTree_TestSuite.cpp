#include "PeriodicSpatialTree_TestSuite.h"
#include "PeriodicSpatialTree.h"
#include "Moon.h"
#include "TestUtils.h"

bool testAddDeleteMoon()
{
  Moon moonA;
  {
    moonA.x = 0.1;
    moonA.y = 0.1;
    moonA.r = 0.05;
  }

  Moon moonB;
  {
    moonB.x = 0.96;
    moonB.y = 0.96;
    moonB.r = 0.05;
  }

  Moon moonC;
  {
    moonC.x = 0.3;
    moonC.y = 0.5;
    moonC.r = 0.05;
  }

  PeriodicDomain domain(0.0,1.0,0.0,1.0);
  PeriodicSpatialTree tree(domain, 0.1);

  tree.add(&moonA);
  tree.add(&moonB);
  tree.add(&moonC);

  ASSERT_EQUAL(3,tree.numberOfMoons());
  
  tree.clear();

  ASSERT_EQUAL(0,tree.numberOfMoons());

  return true;
}

bool testCalculateHash()
{
  Moon moonA;
  {
    moonA.x = 0.1;
    moonA.y = 0.1;
    moonA.r = 0.05;
  }

  Moon moonB;
  {
    moonB.x = 0.96;
    moonB.y = 0.96;
    moonB.r = 0.05;
  }

  Moon moonC;
  {
    moonC.x = 0.3;
    moonC.y = 0.5;
    moonC.r = 0.05;
  }

  Moon moonD;
  {
    moonD.x = 0.96;
    moonD.y = 0.06;
    moonD.r = 0.05;
  }

  PeriodicSpatialTree treeA(PeriodicDomain(0.0,1.0,0.0,1.0), 0.1);

  ASSERT_EQUAL(0, treeA.calculateHash(&moonA));
  ASSERT_EQUAL(80, treeA.calculateHash(&moonB));
  ASSERT_EQUAL(38, treeA.calculateHash(&moonC));
  ASSERT_EQUAL(8, treeA.calculateHash(&moonD));

  // translating the domain by its width (or height) should not alter the hashes
  // of the moons (due to the periodicity).
  PeriodicSpatialTree treeB(PeriodicDomain(-1.0,0.0,-1.0,0.0), 0.1);

  ASSERT_EQUAL(0, treeB.calculateHash(&moonA));
  ASSERT_EQUAL(80, treeB.calculateHash(&moonB));
  ASSERT_EQUAL(38, treeA.calculateHash(&moonC));
  ASSERT_EQUAL(8, treeA.calculateHash(&moonD));

  return true;
}

bool testResolveCollisionWith()
{
  Moon moonA;
  {
    moonA.u = -1.0;
    moonA.v = 0.0;
    moonA.x = 0.06;
    moonA.y = 0.06;
    moonA.r = 0.05;
  }

  Moon moonB;
  {
    moonB.u = 1.0;
    moonB.v = 0.0;
    moonB.x = 0.97;
    moonB.y = 0.06;
    moonB.r = 0.05;
  }

  PeriodicSpatialTree tree(PeriodicDomain(0.0,1.0,0.0,1.0), 0.1);

  tree.add(&moonA);
  tree.add(&moonB);

  const unsigned int i = 8;
  const unsigned int j = 0;

  tree.resolveCollisionWith(&moonB,i+1,j);

  ASSERT_EQUAL(1.0, moonA.u);
  ASSERT_EQUAL(-1.0, moonB.u);

  return true;
}

TestSuite PeriodicSpatialTree_TestSuite::getSuite()
{
  TestSuite suite;
  
  suite.push_back(TEST(PeriodicSpatialTree,testAddDeleteMoon));
  suite.push_back(TEST(PeriodicSpatialTree,testCalculateHash));
  suite.push_back(TEST(PeriodicSpatialTree,testResolveCollisionWith));

  return suite;
}


