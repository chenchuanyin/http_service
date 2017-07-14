#ifndef __UTIL_H__
#define __UTIL_H__

#include <map>
#include <string>

namespace Util {
    std::string getSources(const std::string &source);

    void split(const std::string &query, const std::string &separator, std::vector<std::string> &array);

    std::string getUUIDFromName(const std::string &uri);
}
#endif // __UTIL_H__
