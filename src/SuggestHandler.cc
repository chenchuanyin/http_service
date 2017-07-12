#include "SuggestHandler.h"

#include <Poco/Logger.h>

void SuggestHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                   Poco::Net::HTTPServerResponse &response) {
    Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                     request.getHost(),
                                     request.getMethod(),
                                     request.getURI());
    response.sendBuffer("hello world", strlen("hello world"));
}
