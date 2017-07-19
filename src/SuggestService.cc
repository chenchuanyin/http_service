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
    Util::checkKeyValueForParam(param_, "route", "");
    route = param_["route"];
    std::string paramStr = genRequestParam(param_);
    EngineRequestReply reply = enginePool_->handleRequest(paramStr, route, false);
    return reply;
}

std::string SuggestService::genRequestParam(const nlohmann::json &param) {
    nlohmann::json json;
    json["text"] = param_["text"];
    json["iskeepalive"] = Environment::Instance().getInt("suggest.iskeepalive");

    LOG_INFO << "param:" << json << "\n";
    return json.dump();
}
