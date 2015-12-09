#include "ArgumentHandler.h"
#include <cstring>
#include <iostream>
#include <getopt.h>

namespace {
  struct option long_options[] = {
    {"config",    required_argument, 0, 'c'},
    {"resources", required_argument, 0, 'r'},
    {0, 0, 0, 0}
  };
}

ArgumentHandler::ArgumentHandler(int argc, char** argv) :
  m_runTests(false),
  m_loadConfig(false),
  m_debug(false)
{
  int option_index = 0;

  while(true){
    int c = getopt_long(argc, argv, "c:r:", ::long_options, &option_index);

    if(c == -1){
      break;
    }

    switch(c){
      case 'c':
        m_config.reset(new std::string(optarg));
        break;
      case 'r':
        m_resource_path.reset(new std::string(optarg));
        break;
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

bool ArgumentHandler::debug() const
{
  return m_debug;
}

std::string ArgumentHandler::getConfiguration() const
{
  return *m_config.get();
}

std::string ArgumentHandler::getResourcePath() const
{
  return *m_resource_path.get();
}

