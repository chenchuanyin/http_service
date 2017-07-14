#include "RedisPool.h"

#include "Log.h"
#include "RedisClient.h"

RedisPool::RedisPool() {
    LOG_DEBUG << "construct\n";
    pool_.clear();
    random_.seed();
}

RedisPool::~RedisPool() {
    LOG_DEBUG << "destruct\n";
    pool_.clear();
}

bool RedisPool::addClient(const std::string &host, int port, const std::string &db) {
    Poco::AutoPtr<RedisClient> redisClientPtr = new RedisClient(host, port, db);
    std::string clientName = Poco::format("%s:%d:%s", host, port, db);
    if (redisClientPtr->isConnected()) {
        LOG_INFO << "redis pool add new client:" << clientName << "\n";
        pool_.push_back(redisClientPtr);
        return true;
    } else {
        LOG_ERROR << "redis[" << clientName << "] not online\n";
        return false;
    }
}

bool RedisPool::addClient(const std::string &host, int port) {
    Poco::AutoPtr<RedisClient> redisClientPtr = new RedisClient(host, port);
    std::string clientName = Poco::format("%s:%d", host, port);
    if (redisClientPtr->isConnected()) {
        LOG_INFO << "redis pool add new client:" << clientName << "\n";
        pool_.push_back(redisClientPtr);
        return true;
    } else {
        LOG_ERROR << "redis[" << clientName << "] not online\n";
        return false;
    }
}

Poco::AutoPtr<RedisClient> RedisPool::getClient(const std::string &host, const std::string &port) {
    LOG_INFO << "host:"<< host << ", port:" << port << "\n";
    std::string key = Poco::format("%s:%s", host, port);
    auto it = std::find(pool_.begin(), pool_.end(), key);
    if (it != pool_.end()) {
        return *it;
    } else {
        if (pool_.size() > 0)
            return pool_[random_.next()%pool_.size()];
        else
            return NULL;
    }
}

