#ifndef __AUTHOR_HELPER_H__
#define __AUTHOR_HELPER_H__

#include "MySQLClient.h"

#include <nlohmann/json.hpp>

class AuthHelper {
public:
    AuthHelper();

    ~AuthHelper();

    void auth(const std::string &appid,
              const std::string &timestamp,
              const std::string &appkey,
              nlohmann::json &result);

private:
    std::string getToken(const std::string &appid, const std::string &timestamp, const std::string &appkey);

    void load_appkeys();

    void connectToMySQL();

private:
    int timespan_;
    bool needAuth_;
    Poco::AutoPtr<MySQLClient> mysqlClient_;
};

#endif // __AUTHOR_HELPER_H__
