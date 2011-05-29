#include <stdexcept>
#include <iostream>

#include "Engine.h"
#include "DataStore.h"

int main(int argc, char** argv)
{
  try
  {
    if (argc > 1)
    {
      DataStore::load(argv[1]);
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
