#include "DefaultHandler.h"
#include "Log.h"

#include <nlohmann/json.hpp>

void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                   Poco::Net::HTTPServerResponse &response) {
    LOG_INFO("host:%s, method:%s, uri:%s", request.getHost().c_str(), request.getMethod().c_str(),
             request.getURI().c_str());
    nlohmann::json result;
    result["rc"] = errorCode_;
    result["error"] = errorContent_;
    response.setContentType("application/json");
    response.sendBuffer(result.dump().c_str(), result.dump().size());
}

DefaultHandler::DefaultHandler(const int errorCode, const std::string &errorContent)
        : errorCode_(errorCode), errorContent_(errorContent) {
}
