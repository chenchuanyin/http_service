#ifndef __UTIL_H__
#define __UTIL_H__

#include <map>

bool parseQuery(const std::string& query, std::map<std::string, std::string>& kv);
#endif // __UTIL_H__
