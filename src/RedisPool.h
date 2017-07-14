#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__

#include <Poco/Redis/Client.h>
#include <Poco/RefCountedObject.h>
#include <Poco/Random.h>
#include <vector>

class RedisClient;


class RedisPool : public Poco::RefCountedObject {
public:
    RedisPool();

    ~RedisPool();

    bool addClient(const std::string &host, int port, const std::string &db);

    bool addClient(const std::string &host, int port);

    Poco::AutoPtr<RedisClient> getClient(const std::string &host, const std::string &port);


private:
    RedisPool(const RedisPool &);

    RedisPool &operator=(const RedisPool &);

private:
    std::vector<Poco::AutoPtr<RedisClient>> pool_;
    Poco::Random random_;
};

#endif //  __REDIS_POOL_H__
