#include "SuggestService.h"
#include "Log.h"
#include "Util.h"

SuggestService::SuggestService(nlohmann::json &param, Poco::AutoPtr<EnginePool> enginePool,
                               Poco::AutoPtr<RedisPool> redisPool)
        : param_(param), enginePool_(enginePool), redisPool_(redisPool) {

}


EngineRequestReply SuggestService::operator()() {
    //LOG_DEBUG << "param:" << param_ << "\n";
    std::string route;
    Util::checkKeyValueForParam(param_, "route", "");
    route = param_["route"];
    std::string paramStr = genRequestParam(param_);
    EngineRequestReply reply = enginePool_->handleRequest(paramStr, route, false);
    if (0 == reply.error["rc"]) {
        EngineRequestReply result = dataFilter(reply);
        return result;
    } else {
        return reply;
    }
}

std::string SuggestService::genRequestParam(const nlohmann::json &param) {
    nlohmann::json json;
    json["text"] = param_["text"];
    json["iskeepalive"] = Environment::Instance().getInt("suggest.iskeepalive");

    //LOG_INFO << "param:" << json << "\n";
    return json.dump();
}

EngineRequestReply SuggestService::dataFilter(EngineRequestReply &data) {
    EngineRequestReply result;
    result.error = data.error;
    result.statistics = data.statistics;
    result.engineAddress = data.engineAddress;

    result.engineReply["rc"] = 0;
    if (data.engineReply.find("word") != data.engineReply.end()) {
        result.engineReply["word"] = data.engineReply["word"];
    } else {
        result.engineReply["word"] = "";
    }
    result.engineReply["succesCount"] = data.engineReply["successcount"];
    result.engineReply["failedCount"] = data.engineReply["failedcount"];
    nlohmann::json results = data.engineReply["data"]["results"];
    for (auto i = 0 ; i < results.size(); ++i) {
        nlohmann::json item;
        item["songName"] = results[i]["songName"];
        item["id"] = results[i]["id"];
        item["originalIds"] = results[i]["originalIds"];
        item["originalMark"] = results[i]["originalMark"];
        item["singerName"] = results[i]["singerName"];
        item["albumName"] = results[i]["albumName"];
        item["hot"] = results[i]["hot"];
        item["hotLevel"] = results[i]["hotlevel"];
        item["songList"] = results[i]["songlist"];
        item["publishTime"] = results[i]["publishTime"];
        item["userType"] = results[i]["userType"];
        item["source"] = results[i]["source"];
        result.engineReply["data"]["result"].push_back(item);
    }
    //LOG_DEBUG << result.engineReply << std::endl;
    return result;
}
