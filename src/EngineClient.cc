//
// Created by silver chan on 2017/7/12.
//

#include "EngineClient.h"

#include "Log.h"

#include <Poco/Format.h>

EngineClient::EngineClient(std::string ip, int port) : ip_(ip), port_(port) {
    updateTime();
    try {
        Poco::Net::SocketAddress socketAddress(ip_, port_);
        socket_.connect(socketAddress);
        status_ = socket_.available();
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() <<"\n";
    }
}

EngineClient::~EngineClient() {
    LOG_DEBUG << "destruct\n";
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
    std::string str = Poco::format("%s:%d", ip_, port_);
    return str;
}
