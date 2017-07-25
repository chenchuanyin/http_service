//
// Created by silver chan on 2017/7/12.
//

#include "EngineClient.h"

#include "Log.h"

#include <Poco/Format.h>

EngineClient::EngineClient(std::string host, int port) : host_(host), port_(port) {
    LOG_DEBUG("construct, host:%s, port:%d", host_.c_str(), port_);

    updateTime();
    try {
        Poco::Net::SocketAddress socketAddress(host_, port_);
        Poco::Timespan timespan(5, 0);
        socket_.connect(socketAddress, timespan);
        status_ = socket_.available();
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s",ex.displayText().c_str());
    }
}

EngineClient::~EngineClient() {
    LOG_DEBUG("destruct, host:%s, port:%d", host_.c_str(), port_);

    if (socket_.available()) {
        socket_.shutdown();
        socket_.close();
    }
}

bool EngineClient::timeLimit() {
    Poco::Timestamp::TimeVal thatTimestamp = timestamp_.raw();
    timestamp_.update();
    Poco::Timestamp::TimeVal thisTimestamp = timestamp_.raw();
    if ((thisTimestamp - thatTimestamp) / 1000000 > 10) {
        return true;
    }
    return false;
}

void EngineClient::updateTime() {
    timestamp_.update();
}

std::string EngineClient::toString() const {
    std::string str = Poco::format("%s:%d", host_, port_);
    return str;
}

bool EngineClient::operator==(const EngineClient &other) {
    return other.port() == this->port() && 0 == other.host().compare(this->host());
}

bool EngineClient::operator!=(const EngineClient &other) {
    return !(*this == other);
}

bool EngineClient::operator==(const std::string &other) {
    return this->toString().compare(other) == 0;
}

bool EngineClient::operator!=(const std::string &other) {
    return !(*this == other);
}

void EngineClient::reconnect(const std::string &host, const int port) {
    socket_.close();
    Poco::Net::SocketAddress socketAddress(host, port);
    socket_.connect(socketAddress);
}

void EngineClient::reconnect() {
    reconnect(host_, port_);
}

bool operator==(Poco::AutoPtr<EngineClient> lhs, const std::string &rhs) {
    return *lhs == rhs;
}

bool operator!=(Poco::AutoPtr<EngineClient> lhs, const std::string &rhs) {
    return !(lhs == rhs);
}
