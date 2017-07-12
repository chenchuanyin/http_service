#include "RedisPool.h"

#include "Log.h"
#include "RedisClient.h"

RedisPool::RedisPool() {
    pool_.clear();
}

RedisPool::~RedisPool() {
    LOG_DEBUG << "destruct\n";
    for (auto i = pool_.begin(); i != pool_.end(); ++i) {
        delete i->second;
    }
    pool_.clear();
}

bool RedisPool::addClient(const std::string &host, int port, const std::string &db) {
    RedisClient *redisClientPtr = new RedisClient(host, port, db);
    std::string clientName = Poco::format("%s:%d:%s", host, port, db);
    if (redisClientPtr->isConnected()) {
        LOG_INFO << "redis pool add new client:" << clientName << "\n";
        pool_.insert(std::pair<std::string, RedisClient *>(clientName, redisClientPtr));
        return true;
    } else {
        LOG_ERROR << "redis[" << clientName << " not online\n";
        return false;
    }
}

bool RedisPool::addClient(const std::string &host, int port) {
    RedisClient *redisClientPtr = new RedisClient(host, port);
    std::string clientName = Poco::format("%s:%d", host, port);
    if (redisClientPtr->isConnected()) {
        LOG_INFO << "redis pool add new client:" << clientName << "\n";
        pool_.insert(std::pair<std::string, RedisClient *>(clientName, redisClientPtr));
        return true;
    } else {
        LOG_ERROR << "redis[" << clientName << "] not online\n";
        return false;
    }
}
