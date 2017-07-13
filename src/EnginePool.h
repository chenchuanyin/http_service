//
// Created by silver chan on 2017/7/12.
//

#ifndef __ENGINE_POOL_H__
#define __ENGINE_POOL_H__

#include "EngineRequestReply.h"

#include <Poco/Timestamp.h>
#include <Poco/RefCountedObject.h>

class EngineClient;

class EnginePool  : public Poco::RefCountedObject {
public:
    EnginePool();

    ~EnginePool();

    bool addClient(const std::string &ip, const int port);

    bool delClient(const std::string &ip, const int port);

    EngineClient* getClient(const std::string &key);

    EngineRequestReply handleRequest(const std::string &param, const std::string &route);

private:
    std::string genRequestData(const std::string &request);

    std::string genResponseData(const std::string &response);

    Poco::Timestamp::TimeVal getCurrentTime();

    Poco::UInt32 getReplyDataSectionSize(EngineClient * client);

private:
    std::map<std::string, EngineClient *> pool_;
    Poco::Timestamp timestamp_;
};


#endif // __ENGINE_POOL_H__
