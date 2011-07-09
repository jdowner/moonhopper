#include <stdexcept>
#include <iostream>

#include "Engine.h"
#include "DataStore.h"
#include "ArgumentHandler.h"
#include "TestDriver.h"

int main(int argc, char** argv)
{
  try
  {
    ArgumentHandler arguments(argc, argv);

    if (arguments.testing())
    {
      TestDriver driver;
      driver.runTests();
    }
    else
    {
      if (arguments.configuration())
      {
        DataStore::load(arguments.getConfiguration());
      }
      else
      {
        DataStore::load(arguments.getResourcePath() + "default.cfg");
      }

      DataStore::set("Debug", arguments.debug());
      DataStore::set("ResourcePath", arguments.getResourcePath());

      Engine engine;

      engine.init();

      while (engine.running())
      {
        engine.update();
        engine.render();
        engine.sleep();
      }

      engine.shutdown();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Encountered unrecognized exception" << std::endl;
  }

  return 0;
}
