//
// Created by silver chan on 2017/7/12.
//

#ifndef __ENGINE_POOL_H__
#define __ENGINE_POOL_H__

#include "EngineRequestReply.h"
#include "EngineClient.h"
#include <Poco/Timestamp.h>
#include <Poco/Random.h>
#include <Poco/RefCountedObject.h>


class EnginePool : public Poco::RefCountedObject {
public:
    EnginePool();

    ~EnginePool();

    bool addClient(const std::string &ip, const int port, bool isSearch);

    bool delClient(const std::string &ip, const int port, bool isSearch);

    Poco::AutoPtr<EngineClient> getClient(const std::string &address, bool isSearch);

    EngineRequestReply handleRequest(const std::string &param, const std::string &route, bool isSearchRequest);

private:
    std::string genSearchRequestData(const std::string &request);

    std::string genSuggestRequestData(const std::string &request);


    Poco::Timestamp::TimeVal getCurrentTime();

    Poco::UInt32 getReplyDataSectionSize(EngineClient *client, bool isSearchRequest);

private:
    std::vector<Poco::AutoPtr<EngineClient>> searchPool_;
    std::vector<Poco::AutoPtr<EngineClient> > suggestPool_;
    Poco::Timestamp timestamp_;
    Poco::Random random_;
};


#endif // __ENGINE_POOL_H__
