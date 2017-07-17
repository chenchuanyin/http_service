#include "DefaultHandler.h"
#include "Log.h"
#include "Define.h"

#include <nlohmann/json.hpp>

void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                   Poco::Net::HTTPServerResponse &response) {
    LOG_INFO << "handleRequest, host:" << request.getHost()
             << ", method:" << request.getMethod()
             << ", uri:" << request.getURI() << "\n";
    nlohmann::json result;
    result["rc"] = errorCode_;
    result["error"] = errorContent_;
    response.setContentType("application/json");
    response.sendBuffer(result.dump().c_str(), result.dump().size());
}

DefaultHandler::DefaultHandler(const int errorCode, const std::string &errorContent)
        : errorCode_(errorCode), errorContent_(errorContent) {

}
