#ifndef __SEARCH_HEADLER_H__
#define __SEARCH_HEADLER_H__

#include "EnginePool.h"
#include "RedisPool.h"
#include <nlohmann/json.hpp>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>


class HttpGetRequestHandler : public Poco::Net::HTTPRequestHandler {
public:
    HttpGetRequestHandler(Poco::AutoPtr<EnginePool> enginePool, Poco::AutoPtr<RedisPool> redisPool);

    void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

    bool checkParam(nlohmann::json &param, nlohmann::json &result);

    void convertParam(nlohmann::json &param);

    void handle(nlohmann::json &param, nlohmann::json &result);

private:
    Poco::AutoPtr<EnginePool> enginePool_;
    Poco::AutoPtr<RedisPool> redisPool_;

    std::string genSearchRequest(const nlohmann::json &param);
};

#endif // __SEARCH_HEADLER_H__
