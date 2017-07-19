//
// Created by silver chan on 2017/7/17.
//

#include "SearchSongService.h"

#include "Define.h"
#include "Log.h"
#include "Util.h"


SearchSongService::SearchSongService(nlohmann::json &param, Poco::AutoPtr<EnginePool> enginePool,
                                     Poco::AutoPtr<RedisPool> redisPool)
        : param_(param), enginePool_(enginePool), redisPool_(redisPool) {

}

std::string SearchSongService::genRequestParam(const nlohmann::json &param) {
    LOG_DEBUG << "param:" << param.dump() << "\n";
    nlohmann::json json;
    json["pagestart"] = param["pageIndex"];
    json["countofpage"] = param["pageSize"];
    json["uuid"] = param["uuid"];
    json["text"] = param["text"];
    json["category"] = param["categoryType"];
    json["source"] = param["source"];
    json["istoler"] = param["isCorrect"];
    json["searchtype"] = param["searchType"];
    json["kyfilter"] = param["isCopyright"];
    json["kyonlyvip"] = param["isVip"];
    json["copyright"] = param["copyrightType"];
    json["iskeepalive"] = Environment::Instance().getInt("suggest.iskeepalive");

    LOG_INFO << "request param: " << json << std::endl;
    return json.dump();
}

EngineRequestReply SearchSongService::operator()() {
    LOG_DEBUG << "param:" << param_.dump() << "\n";
    int source = param_["source"];
    int categoryIndex = param_["category"];
    std::string categoryType;
    if (0 == categoryIndex) {
        if (0 == source || 1 == source || 3 == source || 15 == source) {
            categoryType = "kuyin_allsvm";
        } else {
            categoryType = "kuyin_all";
        }
    } else {
        categoryType = definedCategoryTypes[categoryIndex];
    }
    param_["categoryType"] = categoryType;
    Util::checkKeyValueForParam(param_, "route", "");
    std::string route; // = param_["route"];
    std::string paramStr = genRequestParam(param_);

    EngineRequestReply reply = enginePool_->handleRequest(paramStr, route, true);
    return reply;
}
