#include "DefaultHandler.h"
#include "SearchHandler.h"
#include "SuggestHandler.h"
#include "DefaultHandler.h"
#include "Log.h"

#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Format.h>

void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                   Poco::Net::HTTPServerResponse &response) {
  LOG_INFO <<"handleRequest, host:" << request.getHost()
            << ", method:" << request.getMethod()
            << ", uri:" << request.getURI() << "\n";
  std::string responseContent = Poco::format("unknown request(%s)", request.getURI());
  response.sendBuffer(responseContent.data(), responseContent.length());
}
