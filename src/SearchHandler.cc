#include "SearchHandler.h"

#include "Util.h"

#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/URI.h>

void SearchHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                   Poco::Net::HTTPServerResponse &response) {
  Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                   request.getHost(),
                                   request.getMethod(),
                                   request.getURI());
  Poco::URI uri(request.getURI());
  Poco::Logger::root().information("path:%s, query:%s", uri.getPath(),
                                   uri.getQuery());
  for (int i = 0; i < uri.getQueryParameters().size(); ++i) {
      std::pair<std::string, std::string> value = uri.getQueryParameters()[i];
      Poco::Logger::root().information("%s -> %s", value.first, value.second);
  }
  response.sendBuffer("search method", strlen("search method"));
}

bool SearchHandler::checkParams(const std::map<std::string, std::string>& params
                                const std::map<std::string, std::string>& result) {
  if (params.find("appId") == params.end() || params["appId"].empty()) {
    result["rc"] = "1";
    result["error"] = "appId不能为空";
    return false;
  }
  if (params.find("token") == params.end() || params["token"].empty()) {
    result["rc"] = "1";
    result["error"] = "token不能为空";
    return false;
  }
  if (params.find("timestamp") == params.end() || params["timestamp"].empty()) {
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

  // TODO 验证请求是否合法
  


}
