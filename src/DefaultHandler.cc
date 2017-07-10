#include "DefaultHandler.h"
#include "SearchHandler.h"
#include "SuggestHandler.h"
#include "DefaultHandler.h"

#include <Poco/Logger.h>
#include <Poco/String.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Format.h>

void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                   Poco::Net::HTTPServerResponse &response) {
  Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                   request.getHost(),
                                   request.getMethod(),
                                   request.getURI());
  std::string responseContent = Poco::format("unknown request(%s)", request.getURI());
  response.sendBuffer(responseContent.data(), responseContent.length());
}
