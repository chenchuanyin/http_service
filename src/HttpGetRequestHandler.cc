#include "HttpGetRequestHandler.h"

#include "Define.h"
#include "Util.h"
#include "Log.h"
#include "AuthHelper.h"
#include "SearchSongService.h"
#include "SuggestService.h"

#include <nlohmann/json.hpp>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>


HttpGetRequestHandler::HttpGetRequestHandler(Poco::AutoPtr<EnginePool> enginePool, Poco::AutoPtr<RedisPool> redisPool)
        : enginePool_(enginePool), redisPool_(redisPool) {

}

void HttpGetRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                          Poco::Net::HTTPServerResponse &response) {
    LOG_INFO << "handleRequest, host:" << request.getHost()
             << ", method:" << request.getMethod()
             << ", uri:" << request.getURI() << "\n";
    response.setContentType("application/json");
    response.setKeepAlive(false);
    try {
        Poco::URI uri(request.getURI());
        LOG_INFO << "path: " << uri.getPath()
                 << ", query:" << uri.getQuery() << "\n";
        nlohmann::json param;
        param["path"] = uri.getPath();
        param["uuid"] = Util::getUUIDFromName(uri.toString());

        for (int i = 0; i < uri.getQueryParameters().size(); ++i) {
            std::pair<std::string, std::string> value = uri.getQueryParameters()[i];
            param[value.first] = value.second;
        }
        nlohmann::json result;
        if (checkParam(param, result)) {
            convertParam(param);
            handle(param, result);
            if (result.find("fromCache") != result.end()) {
                result.erase("fromCache");
            }
        } else {
            LOG_ERROR << "wrong request,result:" << result.dump() << "\n";
        }
        response.sendBuffer(result.dump().c_str(), result.dump().size());
    } catch (std::exception &ex) {
        LOG_ERROR << ex.what() << "\n";
        nlohmann::json result;
        result["rc"] = UNKNOWN_ERROR;
        result["error"] = ex.what();
        response.sendBuffer(result.dump().c_str(), result.dump().size());
    }
}

void HttpGetRequestHandler::handle(nlohmann::json &param, nlohmann::json &result) {
    LOG_DEBUG << "param:" << param.dump() << "\n";
    nlohmann::json cacheKey;
    cacheKey["versionNo"] = param["versionNo"];
    cacheKey["path"] = param["path"];
    cacheKey["pageSize"] = param["pageSize"];
    cacheKey["appId"] = param["appId"];
    cacheKey["isCorrect"] = param["isCorrect"];
    cacheKey["expire"] = param["expire"];
    cacheKey["isCopyright"] = param["isCopyright"];
    cacheKey["bestShowType"] = param["bestShowType"];
    cacheKey["searchSwitch"] = param["searchSwitch"];
    cacheKey["size"] = param["size"];
    cacheKey["isSAM"] = param["isSAM"];
    cacheKey["mst"] = param["mst"];
    std::string path = param["path"];
    if (path.find("searchsong") != std::string::npos) {
        std::string sort = param["sort"];
        if (sort.compare("0") == 0)
            cacheKey["sort"] = 0;
        else
            cacheKey["sort"] = 1;
    }
    std::string queryid = Util::genMD5(cacheKey.dump());
    param["uuid"] = param["sid"];
    param["queryId"] = queryid;
    if (1 == Environment::Instance().getInt("default.use_query_cache")) {
        //Todo route设置
        std::string route;
        nlohmann::json cache = redisPool_->getCache(queryid, route);
        if (cache.empty()) {
            EngineRequestReply reply;
            param["source"] = Util::getSources("010000");
            if ("/verticalSearch/api/json/search" == param["path"]) {
                SearchSongService searchSongService(param, enginePool_, redisPool_);
                reply = searchSongService();
            } else {
                SuggestService suggestService(param, enginePool_, redisPool_);
                reply = suggestService();
            }
            result = reply.engineReply;
            result["fromCache"] = 0;
            if (result["rc"] == SUCCESS)
                redisPool_->setex(route, queryid, reply.engineReply.dump(), 240);
        } else {
            result = cache;
            result["fromCache"] = 1;
            redisPool_->setex(route, queryid, cache, 240);
        }
    } else {
        EngineRequestReply reply;
        if ("/verticalSearch/api/json/search" == param["path"]) {
            SearchSongService searchSongService(param, enginePool_, redisPool_);
            reply = searchSongService();
        } else if ("/verticalSearch/api/json/suggest" == param["path"]) {
            SuggestService suggestService(param, enginePool_, redisPool_);
            reply = suggestService();
        }
        if (reply.error["rc"] == SUCCESS) {
            result = reply.engineReply;
            result["fromCache"] = 0;
        } else {
            result = reply.error;
        }
    }
}


bool HttpGetRequestHandler::checkParam(nlohmann::json &param, nlohmann::json &result) {
    std::string apiVecStr = Environment::Instance().getString("http.api_list");
    std::vector<std::string> apiVec;
    Util::split(apiVecStr, ",", apiVec);
    if (std::find(apiVec.begin(), apiVec.end(), param["path"]) == apiVec.end()) {
        result["rc"] = ERROR_SERVICE;
        result["error"] = "请求的服务不存在";
        return false;
    }

    if (param["appId"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "appId不能为空";
        return false;
    }

    if (param["token"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "token不能为空";
        return false;
    }

    if (param["timestamp"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "timestamp不能为空";
        return false;
    }

    Util::checkKeyValueForParam(param, "isVip", 0);
    if (param["isVip"] != 0 && param["isVip"] != 1) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "isVip的值不正确，要求0或1";
        return false;
    }

    AuthHelper authHelper;
    authHelper.auth(param["appId"], param["timestamp"], param["token"], result);
    if (0 != result["rc"]) {
        result["error"] = "身份验证失败";
        return false;
    }
    if (param["sid"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "sid不能为空";
        return false;
    }
    if (param["sessionId"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "sessionId不能为空";
        return false;
    }
    if (param["text"].empty()) {
        result["rc"] = ERROR_PARAM;
        result["error"] = "text不能为空";
        return false;
    }
    Util::checkKeyValueForParam(param, "pageIndex", 1);
    if (param["pageIndex"] < 1) {
        param["pageIndex"] = 1;
    }
    Util::checkKeyValueForParam(param, "pageSize", 20);
    if (param["pageSize"] > 20) {
        param["pageSize"] = 20;
    }
    Util::checkKeyValueForParam(param, "source", 15, definedSources);
    Util::checkKeyValueForParam(param, "category", 0, definedCategorys);
    result["rc"] = SUCCESS;
    return true;
}


void HttpGetRequestHandler::convertParam(nlohmann::json &param) {
    Util::checkKeyValueForParam(param, "isCopyright", 0, definedCopyrights);
    Util::checkKeyValueForParam(param, "copyrightType", 0, definedCopyrightTypes);
    Util::checkKeyValueForParam(param, "searchType", 2, definedSearchTypes);
    Util::checkKeyValueForParam(param, "isCorrect", 1, definedCorrects);
    Util::checkKeyValueForParam(param, "issemantic", 1, definedSemantics);

    Util::checkKeyValueForParam(param, "versionNo", "1.0");
    std::string versionNo = param["versionNo"];
    Poco::toLowerInPlace(versionNo);
    Poco::removeInPlace(versionNo, 'v');
    param["versionNo"] = versionNo;
}

std::string HttpGetRequestHandler::genSearchRequest(const nlohmann::json &param) {
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
    LOG_INFO << "request param: " << json << std::endl;
    return json.dump();
}


