//
// Created by silver chan on 2017/7/12.
//

#include "RedisClient.h"

#include "Log.h"

#include <Poco/Redis/Command.h>
#include <Poco/Redis/Exception.h>

RedisClient::RedisClient(std::string host, int port, std::string db)
        : host_(host), port_(port), db_(db), isConnected_(false) {
    LOG_DEBUG("construct, host:%s, port:%d, db:%s", host_.c_str(), port_, db_.c_str());
    try {
        Poco::Timespan timespan(5, 0);
        client_.connect(host_, port_, timespan);
        isConnected_ = true;
        selectDB(db);
        LOG_INFO("connected to redis server[%s:%d:%s]", host_.c_str(), port_, db_.c_str());
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

RedisClient::RedisClient(std::string host, int port)
        : host_(host), port_(port), isConnected_(false) {
    LOG_DEBUG("construct, host:%s, port:%d", host_.c_str(), port_);
    try {
        client_.connect(host_, port_);
        isConnected_ = true;
        db_ = "0";
        LOG_INFO("connected to redis server[%s:%d:%s]", host_.c_str(), port_, db_.c_str());
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

RedisClient::~RedisClient() {
    LOG_DEBUG("destruct, host:%s, port:%d, db:%s", host_.c_str(), port_, db_.c_str());
    if (isConnected_) {
        client_.disconnect();
        isConnected_ = false;
        LOG_INFO("disconnect from redis server[%s:%d:%s]", host_.c_str(), port_, db_.c_str());
    }
}

bool RedisClient::selectDB(std::string db) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Redis::Array cmd;
        cmd << "SELECT" << db;
        std::string result = client_.execute<std::string>(cmd);
        return result.compare("OK") == 0 ? true : false;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }

}

bool RedisClient::sismember(const std::string &key, const std::string &member) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Redis::Command command = Poco::Redis::Command::sismember(key, member);
        std::string result = client_.execute<std::string>(command);
        return result.compare("1") ? true : false;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

bool RedisClient::setex(const std::string &key, const std::string &value, int expire) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Timespan timespan(expire, 0);
        Poco::Redis::Command command = Poco::Redis::Command::set(key, value, true, timespan);
        std::string result = client_.execute<std::string>(command);
        return result.compare("OK") == 0 ? true : false;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

bool RedisClient::set(const std::string &key, const std::string &value) {
    return setex(key, value, 0);
}

bool RedisClient::get(const std::string &key, std::string &value) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Redis::Command command = Poco::Redis::Command::get(key);
        Poco::Redis::BulkString result = client_.execute<Poco::Redis::BulkString>(command);
        value = result.value();
        return true;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

bool RedisClient::lrange(const std::string &key, const int offset, const int limit, Poco::Redis::Array &array) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Redis::Command command = Poco::Redis::Command::lrange(key, offset, offset + limit - 1);
        array = client_.execute<Poco::Redis::Array>(command);
        return true;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

bool RedisClient::rpush(const std::string &key, const std::string &value) {
    if (false == isConnected_) {
        LOG_WARN("redis not connencted, skip");
        return false;
    }

    try {
        Poco::Redis::Command command = Poco::Redis::Command::rpush(key, value);
        client_.execute<std::string>(command);
        return true;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

std::string RedisClient::toString() const {
    return Poco::format("%s:%d:%s", host_, port_, db_);
}

bool RedisClient::operator==(const RedisClient &other) {
    return this->host().compare(other.host()) == 0 && this->port() == other.port()
           && this->db().compare(other.db()) == 0;
}

bool RedisClient::operator==(const std::string &other) {
    return this->toString().compare(other);
}

bool RedisClient::operator!=(const RedisClient &other) {
    return !(*this == other);
}

bool RedisClient::operator!=(const std::string &other) {
    return !(*this == other);
}

bool operator==(Poco::AutoPtr<RedisClient> lhs, const std::string &rhs) {
    return *lhs == rhs;
}

bool operator!=(Poco::AutoPtr<RedisClient> lhs, const std::string &rhs) {
    return !(*lhs == rhs);
}
