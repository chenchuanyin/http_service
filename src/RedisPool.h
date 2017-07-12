#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__

#include <Poco/Redis/Client.h>

class RedisPool {
public:
    RedisPool(std::string host, int port, std::string db);

    RedisPool(std::string host, int port);

    ~RedisPool();

    bool selectDB(std::string db);

private:
    std::string host_;
    int port_;
    std::string db_;
    bool isConnected_;
    Poco::Redis::Client client_;
};

#endif //  __REDIS_POOL_H__
