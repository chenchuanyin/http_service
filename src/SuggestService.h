#ifndef __SUGGEST_HANDLER_H__
#define __SUGGEST_HANDLER_H__

#include "EnginePool.h"
#include "RedisPool.h"

#include <nlohmann/json.hpp>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class SuggestService {
public:
    SuggestService(nlohmann::json &param, Poco::AutoPtr<EnginePool> enginePool, Poco::AutoPtr<RedisPool> redisPool);

    EngineRequestReply operator()();

private:
    nlohmann::json param_;
    Poco::AutoPtr<EnginePool> enginePool_;
    Poco::AutoPtr<RedisPool> redisPool_;
};

#endif // __SUGGEST_HANDLER_H__
