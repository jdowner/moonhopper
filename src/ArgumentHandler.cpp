#include "ArgumentHandler.h"
#include <cstring>

ArgumentHandler::ArgumentHandler(int argc, char** argv) :
  m_runTests(false),
  m_loadConfig(false),
  m_config("")
{
  for (unsigned int index = 1; index < argc; ++index)
  {
    if (strcmp(argv[index], "--test") == 0)
    {
      m_runTests = true;
    }
    else if (strcmp(argv[index], "--config") == 0)
    {
      m_loadConfig = true;
      m_config = std::string(argv[++index]);
    }
    else if (strcmp(argv[index], "--resources") == 0)
    {
    }
    else if (strcmp(argv[index], "--help") == 0)
    {
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
  
