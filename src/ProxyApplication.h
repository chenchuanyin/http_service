//
// Created by silver chan on 2017/7/12.
//

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "RedisPool.h"
#include "EnginePool.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Util/ServerApplication.h>

class ProxyApplication  : public Poco::Util::ServerApplication {
public:
    ProxyApplication(const std::string &configFile = "default.conf");

    ~ProxyApplication();

    void init();

    void uninit();

protected:
    int main(const std::vector<std::string> &args);

private:
    bool initConfig();

    bool initLog();

    bool initService();

    bool initHttpServer();

    bool initRedisPool();

    bool initEnginePool();

    bool addRedis(const std::string &redisAddress);

    ProxyApplication &operator=(const ProxyApplication &);

    ProxyApplication(const ProxyApplication &);

private:
    std::string configFile_;
    Poco::Net::HTTPServer *httpServer_;
    Poco::AutoPtr<RedisPool> redisPool_;
    Poco::AutoPtr<EnginePool> enginePool_;
};


#endif // __APPLICATION_H__
