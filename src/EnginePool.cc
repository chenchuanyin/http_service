//
// Created by silver chan on 2017/7/12.
//

#include "EnginePool.h"

#include "Define.h"
#include "Log.h"
#include "Util.h"

#include <Poco/ByteOrder.h>
#include <fstream>

EnginePool::EnginePool() {
    LOG_DEBUG("construct");
    searchPool_.clear();
    suggestPool_.clear();
    random_.seed();
}

EnginePool::~EnginePool() {
    LOG_DEBUG("destruct");
}

bool EnginePool::addClient(const std::string &ip, const int port, bool isSearch) {
    LOG_INFO("add engine client(%s:%d), isSearch:%s", ip.c_str(), port, isSearch ? "true" : "false");
    Poco::AutoPtr<EngineClient> client = new EngineClient(ip, port);
    if (isSearch) {
        searchPool_.push_back(client);
    } else {
        suggestPool_.push_back(client);

    }
    return true;
}

bool EnginePool::delClient(const std::string &ip, const int port, bool isSearch) {
    LOG_INFO("del engine client(%s:%d), isSearch:%s", ip.c_str(), port, isSearch ? "true" : "false");

    std::string key = Poco::format("%s:%d", ip, port);
    if (isSearch) {
        auto it = std::find(searchPool_.begin(), searchPool_.end(), key);
        if (it != searchPool_.end()) {
            searchPool_.erase(it);
        } else {
            LOG_INFO("search pool not contain a client(%s:%d)", ip.c_str(), port);
        }
    } else {
        auto it = std::find(suggestPool_.begin(), suggestPool_.end(), key);
        if (it != suggestPool_.end()) {
            suggestPool_.erase(it);
        } else {
            LOG_INFO("suggest pool not contain a client(%s:%d)", ip.c_str(), port);
        }
    }
    return true;
}

Poco::AutoPtr<EngineClient> EnginePool::getClient(const std::string &address, bool isSearch) {
    LOG_DEBUG("address:%s, isSearch:%s", address.c_str(), isSearch ? "true" : "false");
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
    Poco::UInt8 preLenBytes[4];
    Poco::UInt32 preLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) (mod.size() + 4 + request.size() + 4));
    Util::int2Bytes(preLen, preLenBytes);
    std::string preLenStr((char *) preLenBytes, 4);
    Poco::UInt8 modLenBytes[4];
    Poco::UInt32 modLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) mod.size());
    Util::int2Bytes(modLen, modLenBytes);
    std::string modLenStr((char *) modLenBytes, 4);
    Poco::UInt8 requestLenBytes[4];
    Poco::UInt32 requestLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) request.size());
    Util::int2Bytes(requestLen, requestLenBytes);
    std::string requestLenStr((char *) requestLenBytes, 4);

    std::stringstream requestDataStream;
    requestDataStream << "SERVER0001" << preLenStr << modLenStr << mod << requestLenStr << request;
    LOG_DEBUG("%s", requestDataStream.str().c_str());
    return requestDataStream.str();
}

std::string EnginePool::genSuggestRequestData(const std::string &request) {
    std::string mod("");
    Poco::UInt8 preLenBytes[4];
    Poco::UInt32 preLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) (mod.size() + 4 + request.size() + 4));
    Util::int2Bytes(preLen, preLenBytes);
    std::string preLenStr((char *) preLenBytes, 4);
    Poco::UInt8 modLenBytes[4];
    Poco::UInt32 modLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) mod.size());
    Util::int2Bytes(modLen, modLenBytes);
    std::string modLenStr((char *) modLenBytes, 4);
    Poco::UInt8 requestLenBytes[4];
    Poco::UInt32 requestLen = Poco::ByteOrder::toBigEndian((Poco::UInt32) request.size());
    Util::int2Bytes(requestLen, requestLenBytes);
    std::string requestLenStr((char *) requestLenBytes, 4);

    std::stringstream requestDataStream;
    requestDataStream << "ASSOSVR100" << preLenStr << modLenStr << mod << requestLenStr << request;
    LOG_DEBUG("%s", requestDataStream.str().c_str());
    return requestDataStream.str();
}


EngineRequestReply EnginePool::handleRequest(const std::string &param,
                                             const std::string &route,
                                             bool isSearchRequest,
                                             int retryCount) {
    LOG_DEBUG("param:%s, route:%s, isSearchRequest:%s, retryCount:%d", param.c_str(), route.c_str(),
              isSearchRequest ? "true" : "false", retryCount);
    Poco::ScopedLock<Poco::Mutex> scopedLock(mutex_);
    EngineRequestReply requestReply;
    Poco::AutoPtr<EngineClient> client = getClient(route, isSearchRequest);
    try {
        Poco::Timestamp::TimeVal startTime = getCurrentTime();
        if (NULL == client) {
            LOG_ERROR("cannot find engine(%s) from engine pool",route.c_str());
            requestReply.error["rc"] = UNAVAILABLE_ENGINE;
            requestReply.error["error"] = "内部服务异常";
            return requestReply;
        }
        requestReply.statistics["initTime"] = (getCurrentTime() - startTime) / MICROSECONDS_PER_SECOND;
        std::string request = isSearchRequest ? genSearchRequestData(param) : genSuggestRequestData(param);
        startTime = getCurrentTime();

        client->socket().sendBytes(request.c_str(), request.size());
        requestReply.statistics["sendTime"] = (getCurrentTime() - startTime) / MICROSECONDS_PER_SECOND;
        startTime = getCurrentTime();
        Poco::UInt32 replyDataSectionSize = getReplyDataSectionSize(client, isSearchRequest);
        if (0 == replyDataSectionSize) {
            client->reconnect();
            return handleRequest(param, route, isSearchRequest, ++retryCount);
        }

        char replyDataSection[replyDataSectionSize];
        int recvLength = 0;
        int allRecvLength = 0;
        int remainLength = replyDataSectionSize - allRecvLength;
        while ((recvLength = client->socket().receiveBytes(replyDataSection + allRecvLength,
                                                           sizeof(replyDataSection + allRecvLength))) !=
               remainLength) {
            allRecvLength += recvLength;
            remainLength = replyDataSectionSize - allRecvLength;
        }
        std::string replyData(replyDataSection, replyDataSectionSize);
        LOG_DEBUG("reply data:%s", replyData.c_str());
        requestReply.engineReply = nlohmann::json::parse(replyData);
        std::stringstream engineReplySS;
        engineReplySS << requestReply.engineReply;
        LOG_DEBUG("reply json:%s", engineReplySS.str().c_str());
        requestReply.statistics["recvTime"] = (getCurrentTime() - startTime) / MICROSECONDS_PER_SECOND;
        requestReply.error["rc"] = SUCCESS;
        return requestReply;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
        if (3 < retryCount) {
            requestReply.error["rc"] = UNAVAILABLE_ENGINE;
            requestReply.error["error"] = ex.displayText();
            return requestReply;
        } else {
            client->reconnect();
            handleRequest(param, route, isSearchRequest, ++retryCount);
        }
    }
}

Poco::UInt32 EnginePool::getReplyDataSectionSize(Poco::AutoPtr<EngineClient> client, bool /*isSearchRequest*/) {
    char headSeg[10];
    int length = 0;
    if (10 != (length = client->socket().receiveBytes(headSeg, 10))) {
        LOG_ERROR("network exception, get 10 bytes head segment failed(length:%d)", length);
        return 0;
    }
    Poco::UInt32 sizeSeg;
    if (4 != (length = client->socket().receiveBytes((void *) &sizeSeg, 4))) {
        LOG_ERROR("network exception, get 4 bytes data section size segment failed(length:%d", length);
        return 0;
    }
    return Poco::ByteOrder::fromNetwork(sizeSeg);
}

