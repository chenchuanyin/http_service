#ifndef __DEFAULT_HEADLER_H__
#define __DEFAULT_HEADLER_H__

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class DefaultHandler : public Poco::Net::HTTPRequestHandler {
public:
    DefaultHandler(const int errorCode, const std::string &errorContent);

    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response);

private:
    int errorCode_;
    std::string errorContent_;
};

#endif // __DEAULT_HEADLER_H__
