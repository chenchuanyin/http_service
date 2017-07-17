#include "SearchHandler.h"

#include "Define.h"
#include "Util.h"
#include "Log.h"
#include "AuthHelper.h"

#include <nlohmann/json.hpp>
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
        nlohmann::json param;
        param["path"] = uri.getPath();
        param["uuid"] = Util::getUUIDFromName(uri.toString());

        for (int i = 0; i < uri.getQueryParameters().size(); ++i) {
            std::pair<std::string, std::string> value = uri.getQueryParameters()[i];
            param[value.first] = value.second;
        }
        nlohmann::json result;
        checkParam(param, result);
        if (0 == result["rc"]) {
            convertParam(param);
            searchSong(param, result);
        } else {
            LOG_ERROR << "request 不合理\n";
        }
        response.sendBuffer(result.dump().c_str(), result.dump().size());
    } catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
        nlohmann::json result;
        result["rc"] = 5;
        result["error"] = ex.displayText();
        response.sendBuffer(result.dump().c_str(), result.dump().size());
    }
}


bool SearchHandler::checkParam(nlohmann::json &param, nlohmann::json &result) {
    if (param["appId"].empty()) {
        result["rc"] = 1;
        result["error"] = "appId不能为空";
        return false;
    }

    if (param["token"].empty()) {
        result["rc"] = 1;
        result["error"] = "token不能为空";
        return false;
    }

    if (param["timestamp"].empty()) {
        result["rc"] = 1;
        result["error"] = "timestamp不能为空";
        return false;
    }

    if (param.find("isVip") == param.end() ||
        (param["isVip"] != "0" && param["isVip"] != "1")) {
        result["rc"] = 1;
        result["error"] = "isVip的值不正确，要求0或1";
        return false;
    }

    AuthHelper authHelper;
    authHelper.auth(param["appId"], param["timestamp"], param["token"], result);
    if (0 != result["rc"]) {
        return false;
    }
    if (param["sid"].empty()) {
        result["rc"] = 1;
        result["error"] = "sid不能为空";
        return false;
    }
    if (param["sessionId"].empty()) {
        result["rc"] = 1;
        result["error"] = "sessionId不能为空";
        return false;
    }
    if (param["text"].empty()) {
        result["rc"] = 1;
        result["error"] = "text不能为空";
        return false;
    }
    if (param["pageIndex"] < 1) {
        param["pageIndex"] = 1;
    } else if (param["pageIndex"] > 20) {
        param["pageIndex"] = 20;
    }
    if (std::find(definedSources.begin(), definedCategorys.end(), param["source"]) == definedSources.end()) {
        param["source"] = 15;
    }
    if (std::find(definedCategorys.begin(), definedCategorys.end(), param["category"]) == definedCategorys.end()) {
        param["category"] = 0;
    }
    return true;
}


void SearchHandler::convertParam(nlohmann::json &param) {
    if (std::find(definedCopyrights.begin(), definedCopyrights.end(), param["isCopyright"]) ==
        definedCopyrights.end()) {
        param["isCopyright"] = 0;
    }
    if (std::find(definedCopyrightTypes.begin(), definedCopyrightTypes.end(), param["copyrightType"])
        == definedCopyrightTypes.end()) {
        param["copyrightType"] = 0;
    }
    if (std::find(definedSearchTypes.begin(), definedSearchTypes.end(), param["searchType"]) ==
        definedSearchTypes.end()) {
        param["searchType"] = 2;
    }
    if (std::find(definedCorrects.begin(), definedCorrects.end(), param["isCorrect"]) == definedCorrects.end()) {
        param["isCorrect"] = 1;
    }
    if (std::find(definedSemantics.begin(), definedSemantics.end(), param["issemantic"]) == definedSemantics.end()) {
        param["issemantic"] = 1;
    }

    std::string versionNo = param["versionNo"];
    Poco::toLowerInPlace(versionNo);
    Poco::removeInPlace(versionNo, 'v');
    param["versionNo"] = versionNo;
}


bool SearchHandler::searchSong(nlohmann::json &param, nlohmann::json &result) {
    int source = param["source"];
    int categoryIndex = param["category"];
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
    param["categoryType"] = categoryType;
    std::string route = param["route"];
    std::string paramStr = getSearchRequestJson(param);

}

std::string SearchHandler::getSearchRequestJson(const nlohmann::json &param) {
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
