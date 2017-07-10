#ifndef __AUTHOR_HELPER_H__
#define __AUHTOR_HELPER_H__

#include <map>

class AuthHelper {
 public:
  void auth(const std::string& appid,
            const std::string& timestamp,
            const std::string& appkey,
            std::map<std::string, std::string> result);
 private:
  std::string getToken(const std::string& appid,
                       const std::string& timestamp,
                       const std::string& appkey);
  void load_appkeys();
}
#endif // __AUTHOR_HELPER_H__
