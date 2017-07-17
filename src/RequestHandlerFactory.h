#ifndef __REQUEST_HANDLER_FACTORY_H__
#define __REQUEST_HANDLER_FACTORY_H__

#include <Poco/Net/HTTPRequestHandlerFactory.h>

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &request);
};

#endif // __REQUEST_HANDLER_FACTORY_H__
