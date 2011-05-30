#include <stdexcept>
#include <iostream>

#include "Engine.h"
#include "DataStore.h"
#include "ArgumentHandler.h"

int main(int argc, char** argv)
{
  try
  {
    ArgumentHandler arguments(argc, argv);

    if (arguments.testing())
    {
    }
    else
    {
      if (arguments.configuration())
      {
        DataStore::load(arguments.getConfiguration());
      }

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
