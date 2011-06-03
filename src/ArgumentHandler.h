#ifndef ARGUMENTHANDLER_H
#define ARGUMENTHANDLER_H

#include <string>

/**
 *
 */
class ArgumentHandler
{
  public:
    ArgumentHandler(int argc, char** argv);

    bool testing() const;
    bool configuration() const;

    std::string getConfiguration() const;
    const std::string& getResourcePath() const;

  private:
    std::string m_resourcePath;
    std::string m_config;
    bool m_loadConfig;
    bool m_runTests;
};

#endif // ARGUMENTHANDLER_H

