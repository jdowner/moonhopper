#include "ArgumentHandler.h"
#include <cstring>

namespace
{
  const std::string DEFAULT_RESOURCE_PATH("../resources/");
}

ArgumentHandler::ArgumentHandler(int argc, char** argv) :
  m_resourcePath(DEFAULT_RESOURCE_PATH),
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
      std::string path(argv[++index]);
      if (path.at(path.size() - 1) != '/')
      {
        path.append(1, '/');
      }

      m_resourcePath = path;
    }
    else if (strcmp(argv[index], "--help") == 0)
    {
      // TODO include a brief description of the available options
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

std::string ArgumentHandler::getConfiguration() const
{
  return m_resourcePath + m_config;
}
  
const std::string& ArgumentHandler::getResourcePath() const
{
  return m_resourcePath;
}

