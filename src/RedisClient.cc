//
// Created by silver chan on 2017/7/12.
//

#include "RedisClient.h"

#include "Log.h"

#include <Poco/Redis/Command.h>

RedisClient::RedisClient(std::string host, int port, std::string db)
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

RedisClient::RedisClient(std::string host, int port)
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

RedisClient::~RedisClient() {
    if (isConnected_) {
        client_.disconnect();
        isConnected_ = false;
        LOG_INFO << "disconnected from redis server[" << host_
                 << ":" << port_ << "]\n";
    }
}

bool RedisClient::selectDB(std::string db) {
    if (false == isConnected_) {
        LOG_WARN << "redis not connencted, skip" << std::endl;
        return false;
    }

    Poco::Redis::Array cmd;
    cmd << "SELECT" << db;
    std::string result = client_.execute<std::string>(cmd);
    if (result.compare("OK") == 0) {
        LOG_INFO << "redis execute success,command:" << cmd.toString() << "\n";
        return true;
    } else {
        LOG_ERROR << "redis execute failed!command:" << cmd.toString() << "\n";
        return false;
    }
}

bool RedisClient::sismember(const std::string& key, const std::string& member) {
    if (false == isConnected_) {
        LOG_WARN << "redis not connencted, skip" << std::endl;
        return false;
    }

    Poco::Redis::Command command = Poco::Redis::Command::sismember(key, member);
    std::string result = client_.execute<std::string>(command);
    return  result.compare("1") ? true : false;
}

bool RedisClient::rpush(const std::string &key, const std::string &value) {
    if (false == isConnected_) {
        LOG_WARN << "redis not connencted, skip" << std::endl;
        return false;
    }
    Poco::Redis::Command command = Poco::Redis::Command::rpush(key, value);
    client_.execute<std::string>(command);
    return true;
}

bool RedisClient::setex(const std::string &key, const std::string &value, int expire) {

}
