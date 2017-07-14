//
// Created by silver chan on 2017/7/12.
//

#include "EnginePool.h"

#include "Log.h"
#include "EngineClient.h"

#include <Poco/ByteOrder.h>

EnginePool::EnginePool() {
    LOG_DEBUG << "construct\n";
    searchPool_.clear();
    suggestPool_.clear();
    random_.seed();
}

EnginePool::~EnginePool() {
    LOG_DEBUG << "destruct\n";
}

bool EnginePool::addClient(const std::string &ip, const int port, bool isSearch) {
    LOG_INFO << "add engine client(" << ip << ":" << port << ")\n";
    Poco::AutoPtr<EngineClient> client = new EngineClient(ip, port);
    if (isSearch) {
        searchPool_.push_back(client);
    } else {
        suggestPool_.push_back(client);

    }
    return true;
}

bool EnginePool::delClient(const std::string &ip, const int port, bool isSearch) {
    LOG_INFO << "del engine client(" << ip << ":" << port << ")\n";
    std::string key = Poco::format("%s:%d", ip, port);
    if (isSearch) {
        auto it = std::find(searchPool_.begin(), searchPool_.end(), key);
        if (it != searchPool_.end()) {
            searchPool_.erase(it);
        } else {
            LOG_INFO << "search pool not contain a client(" << ip << ":" << port << ")\n";
        }
    } else {
        auto it = std::find(suggestPool_.begin(), suggestPool_.end(), key);
        if (it != suggestPool_.end()) {
            suggestPool_.erase(it);
        } else {
            LOG_INFO << "suggest pool not contain a client(" << ip << ":" << port << ")\n";
        }
    }
    return true;
}

Poco::AutoPtr<EngineClient> EnginePool::getClient(const std::string &address, bool isSearch) {
    if (isSearch) {
        auto it = std::find(searchPool_.begin(), searchPool_.end(), address);
        if (it != searchPool_.end()) {
            return *it;
        } else {
            if (searchPool_.size() != 0)
                return searchPool_[random_.next() % searchPool_.size()];
            else
                return NULL;
        }
    } else {
        auto it = std::find(suggestPool_.begin(), suggestPool_.end(), address);
        if (it != suggestPool_.end()) {
            return *it;
        } else {
            if (suggestPool_.size() != 0)
                return suggestPool_[random_.next() % suggestPool_.size()];
            else
                return NULL;
        }
    }
}

Poco::Timestamp::TimeVal EnginePool::getCurrentTime() {
    timestamp_.update();
    return timestamp_.raw();
}

std::string EnginePool::genSearchRequestData(const std::string &request) {
    std::string mod("MOD_GAS");
    Poco::UInt32 preLen = Poco::ByteOrder::toLittleEndian(mod.size() + 4 + request.size() + 4);
    Poco::UInt32 modLen = Poco::ByteOrder::toLittleEndian(mod.size());
    Poco::UInt32 requestLen = Poco::ByteOrder::toLittleEndian(request.size());
    std::string requestData = Poco::format("SERVER0001%ul%ul%s%ul%s", preLen, modLen, mod, requestLen, request);
    return requestData;
}

std::string EnginePool::genSuggestRequestData(const std::string &request) {
    std::string mod("");
    Poco::UInt32 preLen = Poco::ByteOrder::toLittleEndian(mod.size() + 4 + request.size() + 4);
    Poco::UInt32 modLen = Poco::ByteOrder::toLittleEndian(mod.size());
    Poco::UInt32 requestLen = Poco::ByteOrder::toLittleEndian(request.size());
    std::string requestData = Poco::format("ASSOSVR100%ul%ul%s%ul%s", preLen, modLen, mod, requestLen, request);
    return requestData;
}


EngineRequestReply EnginePool::handleRequest(const std::string &param, const std::string &route, bool isSearchRequest) {
    EngineRequestReply requestReply;
    Poco::Timestamp::TimeVal startTime = getCurrentTime();
    EngineClient *client = getClient(route, isSearchRequest);
    if (NULL == client) {
        LOG_ERROR << "cannot find engine(" << route << ") from engine pool\n";
        requestReply.errors["code"] = "ENGINE_EXCEPTION";
        requestReply.errors["message"] = u8"内部服务异常";
        return requestReply;
    }
    requestReply.statistics["initTime"] = (getCurrentTime() - startTime) / 1000000;
    if (!client) {
        requestReply.errors["code"] = "UNKNOWN_EXCEPTION";
        requestReply.errors["message"] = u8"内部服务异常";
        return requestReply;
    }
    std::string request = isSearchRequest ? genSearchRequestData(param) : genSuggestRequestData(param);
    startTime = getCurrentTime();
    client->socket().sendBytes(request.c_str(), request.size());
    requestReply.statistics["sendTime"] = (getCurrentTime() - startTime) / 1000000;
    startTime = getCurrentTime();
    Poco::UInt32 replyDataSectionSize = getReplyDataSectionSize(client, isSearchRequest);
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

Poco::UInt32 EnginePool::getReplyDataSectionSize(EngineClient *client, bool /*isSearchRequest*/) {
    try {
        char headSeg[10];
        if (10 != client->socket().receiveBytes(headSeg, 10)) {
            LOG_ERROR << "network exception, get 10 bytes head segment failed\n";
            return 0;
        }
        Poco::UInt32 sizeSeg;
        if (4 != client->socket().receiveBytes((void *) &sizeSeg, 4)) {
            LOG_ERROR << "network exception, get 4 bytes data section size segment failed\n";
            return 0;
        }
        return Poco::ByteOrder::fromNetwork(sizeSeg);
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}

