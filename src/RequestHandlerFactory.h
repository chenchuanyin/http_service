#ifndef __REQUEST_HANDLER_FACTORY_H__
#define __REQUEST_HANDLER_FACTORY_H__

#include "DefaultHandler.h"
#include "SearchHandler.h"
#include "SuggestHandler.h"
#include <Poco/Logger.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/StringTokenizer.h>
#include <string>
#include <map>

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    virtual Poco::Net::HTTPRequestHandler *
    createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
        Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                         request.getHost(),
                                         request.getMethod(),
                                         request.getURI());
        if ("GET" == request.getMethod()) {
            std::string path;
            std::string query;
            std::map<std::string, std::string> params;
            Poco::StringTokenizer askTokenizer(request.getURI(), "?");
            if (0 == Poco::icompare(askTokenizer[0], "/verticalSearch/api/json/searchsong")) {
                return new SearchHandler;
            } else if (0 == Poco::icompare(askTokenizer[0], "/verticalSearch/api/json/suggest")) {
                return new SuggestHandler;
            }
        }
        return new DefaultHandler;
    }
};

#endif // __REQUEST_HANDLER_FACTORY_H__
