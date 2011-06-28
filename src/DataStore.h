#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <sstream>
#include <map>
#include <cassert>
#include <boost/lexical_cast.hpp>

/**
 *
 */
class DataStore
{
  public:
    static void load(const std::string& filename);
    static bool contains(const std::string& key);

    template <typename T>
    static void set(const std::string& key, const T& value)
    {
      std::ostringstream oss;
      oss << value;
      getInstance().m_data[key] = oss.str();
    }

    template <typename T>
    static T get(const std::string& key)
    {
      assert(contains(key));
      return boost::lexical_cast<T>(getInstance().m_data[key]);
    }

    template <typename T>
    static T get(const std::string& key, const T& defaultValue)
    {
      if (!contains(key))
      {
        return defaultValue;
      }
      return boost::lexical_cast<T>(getInstance().m_data[key]);
    }

    static const char* get(const std::string& key, const char* defaultValue)
    {
      if (!contains(key))
      {
        return defaultValue;
      }
      return getInstance().m_data[key].c_str();
    }

  private:
    DataStore();

    static DataStore& getInstance();

  private:
    std::map<std::string,std::string> m_data;
    static DataStore* datastore;
};

#endif // DATASTORE_H

