#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <cassert>
#include <boost/lexical_cast.hpp>

class DataValue
{
  public:
    template <typename T>
    T as() const {
      T var;
      std::istringstream iss;
      iss.str(m_value);
      iss >> var;
      return var;
    }

    const std::string& get() const noexcept {
      return m_value;
    }

  private:
    std::string m_value;
};

class DataContainer : public DataValue
{
  public:
    typedef std::vector<DataValue>::iterator iterator;
    typedef std::vector<DataValue>::const_iterator const_iterator;

  public:
    const DataValue& append(const DataValue& value){
      m_values.push_back(value);
    }

    iterator begin() { return m_values.begin(); }
    const_iterator begin() const { return m_values.begin(); }

    iterator end() { return m_values.end(); }
    const_iterator end() const { return m_values.end(); }

  private:
    std::vector<DataValue> m_values;
};

class KVStore
{
  private:
    std::map<std::string, DataValue> m_values;
};

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
      if (!contains(key))
      {
        std::stringstream message;
        message << "Unable to find '" << key << "' requested from ";
        message << "the DataStore";
        throw std::runtime_error(message.str());
      }
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

