//
// Created by silver chan on 2017/7/12.
//

#include "EnginePool.h"

#include "Log.h"
#include "EngineClient.h"

#include <Poco/ByteOrder.h>

EnginePool::EnginePool() {
    pool_.clear();
}

EnginePool::~EnginePool() {
    LOG_DEBUG << "destruct\n";
    for (auto i = pool_.begin(); i != pool_.end(); ++i) {
        delete i->second;
    }
}

bool EnginePool::addClient(const std::string &ip, const int port) {
    LOG_INFO  << "add engine client(" << ip << ":" << port << ")\n";
    EngineClient *client = new EngineClient(ip, port);
    std::string clientName = Poco::format("%s:%d", ip, port);
    pool_.insert(std::pair<std::string, EngineClient*>(clientName, client));
    return true;
}

bool EnginePool::delClient(const std::string &ip, const int port) {
    LOG_INFO << "del engine client(" << ip << ":" << port << ")\n";
    std::string key = Poco::format("%s:%d", ip, port);
    if (pool_.find(key) != pool_.end()) {
        delete pool_[key];
        pool_.erase(key);
    }
    else {
        LOG_INFO << "engine pool not contain a client(" << ip << ":" << port << ")\n";
    }
    return true;
}

std::string EnginePool::genRequestData(const std::string &request) {
    std::string mod("MOD_GAS");
    Poco::UInt32 preLen = Poco::ByteOrder::toLittleEndian(mod.size()+4+request.size()+4);
    Poco::UInt32 modLen = Poco::ByteOrder::toLittleEndian(mod.size());
    Poco::UInt32 requestLen = Poco::ByteOrder::toLittleEndian(request.size());
    std::string requestData = Poco::format("SERVER0001%ul%ul%s%ul%s", preLen, modLen, mod, requestLen, request);
    return requestData;
}

EngineRequestReply EnginePool::handleRequest(const std::string &param, const std::string &route) {
    EngineRequestReply requestReply;
    Poco::Timestamp::TimeVal startTime = getCurrentTime();
    EngineClient *client = getClient(route);
    requestReply.statistics["initTime"] = (getCurrentTime() - startTime) / 1000000;
    if (!client) {
        requestReply.errors["code"] = "UNKNOWN_EXCEPTION";
        requestReply.errors["message"] = u8"内部服务异常";
        return requestReply;
    }
    std::string request = genRequestData(param);
    startTime = getCurrentTime();
    client->socket().sendBytes(request.c_str(), request.size());
    requestReply.statistics["sendTime"] = (getCurrentTime() - startTime) / 1000000;
    startTime = getCurrentTime();
    Poco::UInt32 replyDataSectionSize = getReplyDataSectionSize(client);
    char replyDataSection[replyDataSectionSize];
    try {
        if (replyDataSectionSize != client->socket().receiveBytes(replyDataSection, replyDataSectionSize)) {
            LOG_ERROR << "network exception, get data section failed, size:" << replyDataSectionSize << "\n";
            requestReply.errors["code"] = "NETWORK_EXCEPTION";
            requestReply.errors["message"] = u8"内部服务异常";
            return requestReply;
        }
        requestReply.engineReply.append(replyDataSection, replyDataSectionSize);
        requestReply.statistics["recvTime"] = (getCurrentTime() - startTime) / 1000000;
        return requestReply;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }

}

EngineClient *EnginePool::getClient(const std::string &key) {
    bool isFromRoute = false;

    return NULL;
}

Poco::Timestamp::TimeVal EnginePool::getCurrentTime() {
    timestamp_.update();
    return timestamp_.raw();
}

Poco::UInt32 EnginePool::getReplyDataSectionSize(EngineClient *client) {
    try {
        char headSeg[10];
        if (10 != client->socket().receiveBytes(headSeg, 10)) {
            LOG_ERROR << "network exception, get 10 bytes head segment failed\n";
            return 0;
        }
        Poco::UInt32 sizeSeg;
        if (4 != client->socket().receiveBytes((void*)&sizeSeg, 4)) {
            LOG_ERROR << "network exception, get 4 bytes data section size segment failed\n";
            return 0;
        }
        return Poco::ByteOrder::fromNetwork(sizeSeg);
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}


