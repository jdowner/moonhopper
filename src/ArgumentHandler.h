#ifndef ARGUMENTHANDLER_H
#define ARGUMENTHANDLER_H

#include <string>
#include <memory>

/**
 *
 */
class ArgumentHandler
{
  public:
    ArgumentHandler(int argc, char** argv);

    bool testing() const;
    bool configuration() const;
    bool debug() const;

    std::string getConfiguration() const;
    std::string getResourcePath() const;

  private:
    std::unique_ptr<std::string> m_resource_path;
    std::unique_ptr<std::string> m_config;
    bool m_loadConfig;
    bool m_runTests;
    bool m_debug;
};

#endif // ARGUMENTHANDLER_H

