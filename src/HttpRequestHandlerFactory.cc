#include "HttpRequestHandlerFactory.h"

#include "DefaultHandler.h"
#include "HttpGetRequestHandler.h"
#include "Define.h"
#include "Environment.h"
#include "Log.h"
#include <Poco/URI.h>


Poco::Net::HTTPRequestHandler *
HttpRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    LOG_INFO << "handleRequest, host:" << request.getHost()
             << ", method:" << request.getMethod()
             << ", uri:" << request.getURI() << "\n";
    try {
        if ("GET" == request.getMethod()) {
            return new HttpGetRequestHandler(enginePool_, redisPool_);
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

HttpRequestHandlerFactory::HttpRequestHandlerFactory(Poco::AutoPtr<EnginePool> enginePool,
                                                     Poco::AutoPtr<RedisPool> redisPool)
        : enginePool_(enginePool),
          redisPool_(redisPool) {

}

