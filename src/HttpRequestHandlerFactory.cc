#include "HttpRequestHandlerFactory.h"

#include "DefaultHandler.h"
#include "HttpGetRequestHandler.h"
#include "Define.h"
#include "Environment.h"
#include "Log.h"
#include <Poco/URI.h>


Poco::Net::HTTPRequestHandler *
HttpRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &request) {
    LOG_INFO("host:%s, method:%s, uri:%s", request.getHost().c_str(), request.getMethod().c_str(),
             request.getURI().c_str());
    try {
        if ("GET" == request.getMethod()) {
            return new HttpGetRequestHandler(enginePool_, redisPool_);
        } else {
            LOG_ERROR("unsupport method:%s", request.getMethod().c_str());
            return new DefaultHandler(UNKNOWN_METHOD, "unsupport method:" + request.getMethod());
        }
    }
    catch (const Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
        return new DefaultHandler(UNKNOWN_ERROR, ex.displayText());
    }
}

HttpRequestHandlerFactory::HttpRequestHandlerFactory(Poco::AutoPtr<EnginePool> enginePool,
                                                     Poco::AutoPtr<RedisPool> redisPool)
        : enginePool_(enginePool),
          redisPool_(redisPool) {

}

