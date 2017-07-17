//
// Created by silver chan on 2017/7/12.
//

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "RedisPool.h"
#include "EnginePool.h"

#include <Poco/Net/HTTPServer.h>

class Application {
public:
    Application &Intance() {
        static Application app;
        return app;
    }

    Application(const std::string &configFile = "default.conf");

    ~Application();

    bool init();

    void run();

    bool unInit();

private:
    bool initConfig();

    bool initLog();

    bool initService();

    bool initHttpServer();

    bool initRedis();

    bool initEngine();

    bool addRedis(const std::string &redisAddress);

    Application &operator=(const Application &);

    Application(const Application &);

private:
    std::string configFile_;
    Poco::Net::HTTPServer *httpServer_;
    Poco::AutoPtr<RedisPool> redisPool_;
    Poco::AutoPtr<EnginePool> enginePool_;

};


#endif // __APPLICATION_H__
