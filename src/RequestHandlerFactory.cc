#include "RequestHandlerFactory.h"

#include "DefaultHandler.h"
#include "SearchHandler.h"
#include "SuggestHandler.h"
#include "Util.h"
#include "Define.h"
#include "Environment.h"
#include "Log.h"
#include <Poco/URI.h>


Poco::Net::HTTPRequestHandler *RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    Poco::Logger::root().information("handleRequest, host:%s, method:%s, uri:%s",
                                     request.getHost(),
                                     request.getMethod(),
                                     request.getURI());
    try {
        if ("GET" == request.getMethod()) {
            Poco::URI uri(request.getURI());
            std::string apiVecStr = Environment::Instance().getString("http.api_list");
            std::vector<std::string> apiVec;
            Util::split(apiVecStr, ",", apiVec);
            if (std::find(apiVec.begin(), apiVec.end(), uri.getPath()) == apiVec.end()) {
                return new DefaultHandler(NO_SUPPORT_METHOD, "请求的服务不存在");
            }
            for (int i = 0; i < apiVec.size(); ++i) {
                if (0 == Poco::icompare(uri.getPath(), "/verticalSearch/api/json/searchsong")) {
                    return new SearchHandler;
                } else if (0 == Poco::icompare(uri.getPath(), "/verticalSearch/api/json/suggest")) {
                    return new SuggestHandler;
                }
            }
        } else {
            LOG_ERROR << "unsupport method:" << request.getMethod() << "\n";
            return new DefaultHandler(UNKNOWN_METHOD, "unsupport method:" + request.getMethod());
        }
    }
    catch (const Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << "\n";
        return new DefaultHandler(UNKNOWN_ERROR, ex.displayText());
    }
}

