#include "SuggestService.h"
#include "Log.h"
#include "Util.h"

SuggestService::SuggestService(nlohmann::json &param, Poco::AutoPtr<EnginePool> enginePool,
                               Poco::AutoPtr<RedisPool> redisPool)
        : param_(param), enginePool_(enginePool), redisPool_(redisPool) {

}


EngineRequestReply SuggestService::operator()() {
    LOG_DEBUG << "param:" << param_ << "\n";
    std::string route;
    nlohmann::json param;
    param["text"] = param_["text"];
    Util::checkKeyValueForParam(param_, "route", "");
    route = param_["route"];
    EngineRequestReply reply = enginePool_->handleRequest(param.dump(), route, false);
    return reply;
}
