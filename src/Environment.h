#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/AutoPtr.h>

class Environment{
 public:
  static Environment& Instance() {
    static Environment env;
    return env;
  }

  bool initialize(std::string filename);
  std::string getString(const std::string& key);
  int getInt(const std::string& key);
  bool getBool(const std::string& key);
  Poco::AutoPtr<Poco::Util::IniFileConfiguration> getConfiguration() const{
    return configFile_;
  }
  bool isInitialized() const { return isInitialized_; }

 protected:
  Environment();
  ~Environment();
 private:
  Poco::AutoPtr<Poco::Util::IniFileConfiguration> configFile_;
  bool isInitialized_;
 private:
  Environment(const Environment&);
  Environment& operator=(const Environment&);
};
#endif // __ENVIRONMENT_H__
