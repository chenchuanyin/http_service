#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <Poco/Config
class Environment{
 public:
  static Environment& getInstance() {
    static Environment env;
    return env;
  }

 private:

 private:
  Environment(const Environment&);
  Environment& operator=(const Environment&);
}
#endif // __ENVIRONMENT_H__
