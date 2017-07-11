#include "Environment.h"

Environment::Environment() : isInitialized_(false)
{
}

Environment::~Environment()
{
}

bool Environment::initialize(std::string filename)
{
  configFile_ = new Poco::Util::IniFileConfiguration(filename);
  if (NULL == configFile_) {
    isInitialized_ = false;
    return false;
  }
  isInitialized_ = true;
  return true;
}

std::string Environment::getString(const std::string& key)
{
  return configFile_->getString(key);
}

int Environment::getInt(const std::string& key)
{
  return configFile_->getInt(key);
}

bool Environment::getBool(const std::string& key)
{
  return configFile_->getBool(key);
}
