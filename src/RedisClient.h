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

    std::string host() const { return host_; }

    int port() const { return port_; }

    std::string db() const { return db_; }

    bool selectDB(std::string db);

    bool setex(const std::string &key, const std::string &value, int expire = 0);

    bool set(const std::string &key, const std::string &value);

    bool get(const std::string &key, std::string &value);

    bool sismember(const std::string &key, const std::string &member);

    bool rpush(const std::string &key, const std::string &value);

    bool lrange(const std::string &key, const int offset, const int limit, Poco::Redis::Array &array);

    bool isConnected() const { return isConnected_; }

    std::string toString() const;

    bool operator==(const RedisClient &other);

    bool operator!=(const RedisClient &other);

    bool operator==(const std::string& other);

    bool operator!=(const std::string& other);


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

bool operator==(Poco::AutoPtr<RedisClient> lhs, const std::string &rhs);
bool operator!=(Poco::AutoPtr<RedisClient> lhs, const std::string &rhs);

#endif // __REDISCLIENT_H__
