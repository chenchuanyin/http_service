#ifndef __AUTHOR_HELPER_H__
#define __AUTHOR_HELPER_H__

#include "MySQLClient.h"

#include <map>

class AuthHelper {
public:
    AuthHelper();

    ~AuthHelper();

    void auth(const std::string &appid,
              const std::string &timestamp,
              const std::string &appkey,
              std::map<std::string, std::string> &result);

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
