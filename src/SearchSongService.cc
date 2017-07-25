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
    if (param.find("uid") != param.end())
        json["userid"] = param["uid"];
    if (param.find("imei") != param.end())
        json["imei"] = param["imei"];
    if (param.find("imsi") != param.end())
        json["imsi"] = param["imsi"];
    if (param.find("app.name") != param.end())
        json["appName"] = param["app.name"];
    if (param.find("app.ver") != param.end())
        json["appVersion"] = param["app.ver"];
    json["iskeepalive"] = Environment::Instance().getInt("suggest.iskeepalive");

    LOG_INFO << "request param: " << json << std::endl;
    return json.dump();
}

EngineRequestReply SearchSongService::operator()() {
    LOG_DEBUG << "param:" << param_.dump() << "\n";
    int source = param_["source"];
    int categoryIndex = param_["category"];
    std::string categoryType;
    if (0 == source || 1 == source || 3 == source || 15 == source) {
        categoryType = "kuyin_allsvm";
    } else if (0 == categoryIndex) {
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
    if (0 == reply.error["rc"]) {
        EngineRequestReply result = dataFilter(reply);
        return result;
    } else {
        return reply;
    }
}

EngineRequestReply SearchSongService::dataFilter(EngineRequestReply &data) {
    EngineRequestReply result;
    result.error = data.error;
    result.statistics = data.statistics;
    result.engineAddress = data.engineAddress;
    result.engineReply["data"]["total"] = data.engineReply["data"]["total"];
    result.engineReply["rc"] = 0;
    result.engineReply["keyWordType"] = data.engineReply["keywordtype"];
    if (data.engineReply.find("tolerance") != data.engineReply.end()) {
        result.engineReply["correct"] = data.engineReply["tolerance"];
    } else {
        result.engineReply["correct"] = "";
    }
    nlohmann::json results = data.engineReply["data"]["results"];
    for (auto i = 0 ; i < results.size(); ++i) {
        nlohmann::json item;
        item["id"] = results[i]["kyringid"];
        item["sameWorkId"] = results[i]["sameworkid"];
        item["name"] = results[i]["songname"];
        item["neatSongName"] = results[i]["neatsongname"];
        item["realSongName"] = results[i]["realsongname"];
        item["singers"] = results[i]["singernames"];
        item["realSingerName"] = results[i]["realsingername"];
        item["tags"] = results[i]["tagnames"];
        item["copyright"] = results[i]["copyright"];
        item["updateTime"] = results[i]["updatetime"];
        item["createTime"] = results[i]["createtime"];
        item["desc"] = results[i]["desc"];
        item["stickMask"] = results[i]["stickymask"];
        item["hot"] = results[i]["hot"];
        item["totalSetCount"] = results[i]["totalsetcount"];
        item["totalListenCount"] = results[i]["totallistencount"];
        item["recentSetCount"] = results[i]["recentsetcount"];
        item["totalListenCount"] = results[i]["totallistencount"];
        item["recentSetCount"] = results[i]["recentsetcount"];
        item["userType"] = results[i]["usertype"];
        item["quality"] = results[i]["quality"];
        item["isEnabled"] = results[i]["isenabled"];
        item["isVip"] = results[i]["isvip"];
        result.engineReply["data"]["result"].push_back(item);
    }
    LOG_DEBUG << result.engineReply << std::endl;
    return result;
}