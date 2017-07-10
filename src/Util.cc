#include "Util.h"
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>

bool parseQuery(const std::string& query, std::map<std::string, std::string>& kv) {
  if (query.empty())
    return true;
  Poco::StringTokenizer andTokenizer(query, "&");
  for (int i = 0; i < andTokenizer.count(); ++i) {
    Poco::StringTokenizer assignTokenizer(andTokenizer[i], "=");
    if (assignTokenizer.count() == 2)
      kv[assignTokenizer[0]] = assignTokenizer[1];
  }
    return true;
}
