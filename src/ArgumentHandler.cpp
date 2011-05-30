#include "ArgumentHandler.h"
#include <cstring>

ArgumentHandler::ArgumentHandler(int argc, char** argv) :
  m_runTests(false),
  m_loadConfig(false),
  m_config("")
{
  if (argc >= 2)
  {
    if (strcmp(argv[1], "--test") == 0)
    {
      m_runTests = true;
    }
    else if (strcmp(argv[1], "--config") == 0)
    {
      m_loadConfig = true;
      m_config = std::string(argv[2]);
    }
  }
}

bool ArgumentHandler::testing() const
{
  return m_runTests;
}

bool ArgumentHandler::configuration() const
{
  return m_loadConfig;
}

const std::string& ArgumentHandler::getConfiguration() const
{
  return m_config;
}
  
