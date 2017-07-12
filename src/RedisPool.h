#ifndef __REDIS_POOL_H__
#define __REDIS_POOL_H__

#include <Poco/Redis/Client.h>
#include <Poco/RefCountedObject.h>
#include <map>

class RedisClient;


class RedisPool : public Poco::RefCountedObject {
public:
    RedisPool();

    ~RedisPool();

    bool addClient(const std::string &host, int port, const std::string &db);

    bool addClient(const std::string &host, int port);


private:
    RedisPool(const RedisPool &);

    RedisPool &operator=(const RedisPool &);

private:
    std::map<std::string, RedisClient *> pool_;
};

#endif //  __REDIS_POOL_H__
