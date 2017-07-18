#ifndef __UTIL_H__
#define __UTIL_H__

#include <map>
#include <string>
#include <nlohmann/json.hpp>

namespace Util {
    std::string getSources(const std::string &source);

    void split(const std::string &query, const std::string &separator, std::vector<std::string> &array);

    std::string getUUIDFromName(const std::string &uri);

    std::string genMD5(const std::string &data);

    template <typename T>
    void checkKeyValueForParam(nlohmann::json &param, const std::string &key, T defaultValue) {
        if (param.find(key) == param.end() || param[key].is_null())
            param[key] = defaultValue;
    }

    template<typename T>
    void
    checkKeyValueForParam(nlohmann::json &param, const std::string &key, T defaultValue, std::vector<T> definedVec) {
        if (param.find(key) == param.end() || param[key].is_null()
            || std::find(definedVec.begin(), definedVec.end(), param[key]) == definedVec.end()) {
            param[key] = defaultValue;
        }
    }

    void int2Bytes(long value, unsigned char *bytes);
}
#endif // __UTIL_H__
