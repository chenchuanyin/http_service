//
// Created by silver chan on 2017/7/12.
//

#ifndef __REDISCLIENT_H__
#define __REDISCLIENT_H__

#include <Poco/Redis/Client.h>
#include <Poco/Redis/Type.h>
#include <Poco/Redis/Redis.h>
#include <Poco/RefCountedObject.h>

class RedisClient : public Poco::RefCountedObject {
public:
    RedisClient(std::string host, int port, std::string db);

    RedisClient(std::string host, int port);

    ~RedisClient();

    bool selectDB(std::string db);

    bool setex(const std::string &key, const std::string &value, int expire = 0);

    bool set(const std::string &key, const std::string &value);

    bool get(const std::string &key, std::string &value);

    bool sismember(const std::string &key, const std::string &member);

    bool rpush(const std::string &key, const std::string &value);

    bool lrange(const std::string &key, const int offset, const int limit, Poco::Redis::Array &array);

    bool isConnected() const { return isConnected_; }


private:
    std::string host_;
    int port_;
    std::string db_;
    bool isConnected_;
    Poco::Redis::Client client_;
private:
    RedisClient(const RedisClient &);

    RedisClient &operator=(const RedisClient &);
};


#endif // __REDISCLIENT_H__
