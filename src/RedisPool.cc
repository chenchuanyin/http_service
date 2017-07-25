#include "RedisPool.h"

#include "Log.h"
#include "RedisClient.h"

RedisPool::RedisPool() {
    LOG_DEBUG("construct");
    pool_.clear();
    random_.seed();
}

RedisPool::~RedisPool() {
    LOG_DEBUG("destruct");
    pool_.clear();
}

bool RedisPool::addClient(const std::string &host, int port, const std::string &db) {
    Poco::AutoPtr<RedisClient> redisClientPtr = new RedisClient(host, port, db);
    std::string clientName = Poco::format("%s:%d:%s", host, port, db);
    if (redisClientPtr->isConnected()) {
        LOG_INFO("redis pool add new client:%s", clientName.c_str());
        pool_.push_back(redisClientPtr);
        return true;
    } else {
        LOG_ERROR("redis[%s] not online", clientName.c_str());
        return false;
    }
}

bool RedisPool::addClient(const std::string &host, int port) {
    addClient(host, port, "0");
}

Poco::AutoPtr<RedisClient> RedisPool::getClient(const std::string &host, const int port, const std::string &db) {
    LOG_INFO("host:%s, port:%d, db:%s", host.c_str(), port, db.c_str());
    std::string key = Poco::format("%s:%d:%s", host, port, db);
    return getClient(key);
}

Poco::AutoPtr<RedisClient> RedisPool::getClient(const std::string &key) {
    auto it = std::find(pool_.begin(), pool_.end(), key);
    if (it != pool_.end()) {
        return *it;
    } else {
        if (pool_.size() > 0)
            return pool_[random_.next() % pool_.size()];
        else
            return NULL;
    }
}

Poco::AutoPtr<RedisClient> RedisPool::getClient(const std::string &host, const int port) {
    LOG_INFO("host:%s, port:%d", host.c_str(), port);
    std::string key = Poco::format("%s:%s:0", host, port);
    return getClient(key);
}


nlohmann::json RedisPool::getCache(const std::string &key, const std::string &route) {
    std::string value;
    Poco::AutoPtr<RedisClient> client = getClient(route);
    if (client) {
        client->get(key, value);
    }

    return value;
}

void RedisPool::setex(const std::string &route, const std::string &key, const std::string &value, int expire) {
    Poco::AutoPtr<RedisClient> client = getClient(route);
    if (client) {
        client->setex(key, value, expire);
    }
}

