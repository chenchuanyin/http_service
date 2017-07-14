#include "SearchHandler.h"

#include "Define.h"
#include "Util.h"
#include "Log.h"
#include "AuthHelper.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>

void SearchHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                  Poco::Net::HTTPServerResponse &response) {
    Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                     request.getHost(),
                                     request.getMethod(),
                                     request.getURI());
    response.setContentType("application/json");
    try {
        Poco::URI uri(request.getURI());
        LOG_INFO << "path: " << uri.getPath()
                 << ", query:" << uri.getQuery() << "\n";
        std::map<std::string, std::string> params;
        params["path"] = uri.getPath();
        params["uuid"] = Util::getUUIDFromName(uri.toString());

        for (int i = 0; i < uri.getQueryParameters().size(); ++i) {
            std::pair<std::string, std::string> value = uri.getQueryParameters()[i];
            params[value.first] = value.second;
        }
        std::map<std::string, std::string> result;
        checkParams(params, result);
        if (result["rc"].compare("0") == 0) {
            convertParams(params);
            searchSong(params, result);
        } else {
            LOG_ERROR << "request 不合理\n";
        }


    } catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}

bool SearchHandler::checkParams(std::map<std::string, std::string> &params,
                                std::map<std::string, std::string> &result) {
    std::string apiVecStr = Environment::Instance().getString("http.api_list");
    std::vector<std::string> apiVec;
    Util::split(apiVecStr, ",", apiVec);
    if (std::find(apiVec.begin(), apiVec.end(), params["path"]) == apiVec.end()) {
        result["rc"] = "2";
        result["error"] = "请求的服务不存在";
        return false;
    }
    if (params["appId"].empty()) {
        result["rc"] = "1";
        result["error"] = "appId不能为空";
        return false;
    }

    if (params["token"].empty()) {
        result["rc"] = "1";
        result["error"] = "token不能为空";
        return false;
    }

    if (params["timestamp"].empty()) {
        result["rc"] = "1";
        result["error"] = "timestamp不能为空";
        return false;
    }

    if (params.find("isVip") == params.end() ||
        (params["isVip"] != "0" && params["isVip"] != "1")) {
        result["rc"] = "1";
        result["error"] = "isVip的值不正确，要求0或1";
        return false;
    }

    AuthHelper authHelper;
    authHelper.auth(params["appId"], params["timestamp"], params["token"], result);
    if (result["rc"].compare("0") != 0) {
        return false;
    }
    if (params["sid"].empty()) {
        result["rc"] = "1";
        result["error"] = "sid不能为空";
        return false;
    }
    if (params["sessionId"].empty()) {
        result["rc"] = "1";
        result["error"] = "sessionId不能为空";
        return false;
    }
    if (params["text"].empty()) {
        result["rc"] = "1";
        result["error"] = "text不能为空";
        return false;
    }
    if (std::atoi(params["pageIndex"].c_str()) < 1) {
        params["pageIndex"] = "1";
    } else if (std::atoi(params["pageIndex"].c_str()) > 20) {
        params["pageIndex"] = "20";
    }
    if (std::find(definedSources.begin(), definedCategorys.end(), params["source"]) == definedSources.end()) {
        params["source"] = "15";
    }
    if (std::find(definedCategorys.begin(), definedCategorys.end(), params["category"]) == definedCategorys.end()) {
        params["category"] = "0";
    }
    return true;
}

void SearchHandler::convertParams(std::map<std::string, std::string> &params) {
    if (std::find(definedCopyrights.begin(), definedCopyrights.end(), params["isCopyright"]) ==
        definedCopyrights.end()) {
        params["isCopyright"] = "0";
    }
    if (std::find(definedCopyrightTypes.begin(), definedCopyrightTypes.end(), params["copyrightType"])
        == definedCopyrightTypes.end()) {
        params["copyrightType"] = "0";
    }
    if (std::find(definedSearchTypes.begin(), definedSearchTypes.end(), params["searchType"]) ==
        definedSearchTypes.end()) {
        params["searchType"] = "2";
    }
    if (std::find(definedCorrects.begin(), definedCorrects.end(), params["isCorrect"]) == definedCorrects.end()) {
        params["isCorrect"] = "1";
    }
    if (std::find(definedSemantics.begin(), definedSemantics.end(), params["issemantic"]) == definedSemantics.end()) {
        params["issemantic"] = "1";
    }

    Poco::toLowerInPlace(params["versionNo"]);
    Poco::removeInPlace(params["versionNo"], 'v');
}

bool SearchHandler::searchSong(std::map<std::string, std::string> &params, std::map<std::string, std::string> &result) {
    std::string pageIndex = params["pageIndex"];
    std::string pageSize = params["pageSize"];
    int source = std::atoi(params["source"].c_str());
    int categoryIndex = std::atoi(params["category"].c_str());
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
    params["categoryType"] = categoryType;
    std::string route = params["route"];

}

std::string SearchHandler::getSearchRequestJson(std::map<std::string, std::string> &params) {
    rapidjson::StringBuffer stringBuffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
    writer.StartObject();
    writer.Key("pagestart");
    writer.Int(std::atoi(params["pageIndex"].c_str()));
    writer.Key("countofpage");
    writer.Int(std::atoi(params["pageSize"].c_str()));
    writer.Key("uuid");
    writer.String(params["uuid"].c_str());
    writer.Key("text");
    writer.String(params["text"].c_str());
    writer.Key("category");
    writer.String(params["categoryType"].c_str());
    writer.Key("source");
    writer.Int(std::atoi(params["source"].c_str()));
    writer.Key("istoler");
    writer.Int(std::atoi(params["isCorrect"].c_str()));
    writer.Key("searchtype");
    writer.Int(std::atoi(params["searchType"].c_str()));
    writer.Key("kyfilter");
    writer.Int(std::atoi(params["isCopyright"].c_str()));
    writer.Key("kyonlyvip");
    writer.Int(std::atoi(params["isVip"].c_str()));
    writer.Key("copyright");
    writer.Int(std::atoi(params["copyrightType"].c_str()));
    writer.EndObject();
    LOG_INFO << "request param: " << stringBuffer.GetString() << std::endl;
    return stringBuffer.GetString();
}
