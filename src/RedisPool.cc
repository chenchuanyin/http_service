#include "RedisPool.h"

#include "Log.h"

#include <Poco/Redis/Array.h>
#include <Poco/Redis/PoolableConnectionFactory.h>

RedisPool::RedisPool(std::string host, int port, std::string db)
        : host_(host), port_(port), db_(db), isConnected_(false) {
    try {
        client_.connect(host_, port_);
        isConnected_ = true;
        selectDB(db);
        LOG_INFO << "connected to redis server[" << host_
                 << ":" << port_ << ":" << db_ << "]\n";
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << "cannot connect to redis server[" << host_
                  << ":" << port_ << ":" << db_ << "]\n";
    }
}

RedisPool::RedisPool(std::string host, int port)
        : host_(host), port_(port), isConnected_(false) {
    try {
        client_.connect(host_, port_);
        isConnected_ = true;
        db_ = "0";
        LOG_INFO << "connected to redis server[" << host_
                 << ":" << port_ << "]\n";
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << "cannot connect to redis server[" << host_
                  << ":" << port_ << "]:" << ex.displayText() << "\n";
    }
}

RedisPool::~RedisPool() {
    if (isConnected_) {
        client_.disconnect();
        isConnected_ = false;
        LOG_INFO << "disconnected from redis server[" << host_
                 << ":" << port_ << "]\n";
    }
}

bool RedisPool::selectDB(std::string db) {
    if (false == isConnected_) {
        LOG_WARN << "redis not connencted, skip" << std::endl;
        return false;
    }

    Poco::Redis::Array cmd;
    cmd << "SELECT" << db;
    std::string result = client_.execute<std::string>(cmd);
    if (result.compare("OK") == 0) {
        LOG_INFO << "redis execute success\n";
        return true;
    }
    else {
        LOG_ERROR << "redis execute failed!\n";
        return false;
    }
}
