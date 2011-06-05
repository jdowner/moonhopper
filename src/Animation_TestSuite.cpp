#include "Animation_TestSuite.h"
#include <iostream>
#include "Animation.h"
#include "TestUtils.h"

namespace
{
  const std::string testxml("\
<animations>\
<animation id='running' src='running.png'>\
<frame>\
<height>64</height>\
<width>32</width>\
<period>1.0</period>\
<offset>0</offset>\
</frame>\
<frame>\
<height>64</height>\
<width>32</width>\
<period>1.0</period>\
<offset>32</offset>\
</frame>\
</animation>\
</animations>\
  ");

  bool testLoading()
  {
    TiXmlDocument doc;
    doc.Parse(testxml.c_str());

    Animation animation;
    animation.load(doc.RootElement()->FirstChildElement("animation"));

    ASSERT_EQUAL(2,animation.numberOfFrames());
    ASSERT_EQUAL("running.png", animation.getSrc());
    ASSERT(NULL != &animation.getCurrentFrame());

    return true;
  }
}

TestSuite Animation_TestSuite::getSuite()
{
  TestSuite suite;
  suite.push_back(TEST(Animation,testLoading));
  return suite;
}
