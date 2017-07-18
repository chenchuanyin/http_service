#ifndef __REQUEST_HANDLER_FACTORY_H__
#define __REQUEST_HANDLER_FACTORY_H__

#include "EnginePool.h"
#include "RedisPool.h"

#include <Poco/Net/HTTPRequestHandlerFactory.h>

class HttpRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    HttpRequestHandlerFactory(Poco::AutoPtr<EnginePool> enginePool, Poco::AutoPtr<RedisPool> redisPool);

    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);

private:
    Poco::AutoPtr<EnginePool> enginePool_;
    Poco::AutoPtr<RedisPool> redisPool_;
};

#endif // __REQUEST_HANDLER_FACTORY_H__
