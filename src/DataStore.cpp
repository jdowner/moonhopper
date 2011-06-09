#include "DataStore.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

DataStore* DataStore::datastore = NULL;

DataStore::DataStore()
{
}

DataStore& DataStore::getInstance()
{
  if (datastore == NULL)
  {
    datastore = new DataStore;
  }

  return *datastore;
}

void DataStore::load(const std::string& filename)
{
  std::ifstream file(filename.c_str(), std::ifstream::in);

  while (file.good())
  {
    std::string buffer;
    getline(file,buffer);

    if (!buffer.empty() && (buffer[0] != '#'))
    {
      std::string key;
      std::string value;
      size_t index = buffer.find(' ');
      if (index == 0 || index == std::string::npos)
      {
        throw std::runtime_error("Encountered invalid format in datastore file");
      }

      key = buffer.substr(0,index);
      value = buffer.substr(index+1,buffer.size() - index);

      getInstance().m_data[key] = value;
    }
  }

  file.close();
}

bool DataStore::contains(const std::string& key)
{
  const DataStore& datastore = getInstance();
  return datastore.m_data.end() != datastore.m_data.find(key);
}
