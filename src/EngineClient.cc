//
// Created by silver chan on 2017/7/12.
//

#include "EngineClient.h"

#include <Poco/Format.h>

EngineClient::EngineClient(std::string ip, int port) : ip_(ip), port_(port), status_(true) {
    updateTime();
}

EngineClient::~EngineClient() {

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
    std::string str = Poco::format("engine client, ip:%s, port:%d, status:%s",
                                   ip_, port_, status_ ? "true" : "false");
}
