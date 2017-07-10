#ifndef __SEARCH_HEADLER_H__
#define __SEARCH_HEADLER_H__

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <map>

class SearchHandler : public Poco::Net::HTTPRequestHandler {
 public:
  void handleRequest(Poco::Net::HTTPServerRequest &request,
                     Poco::Net::HTTPServerResponse &response);
  bool checkParams(const std::map<std::string, std::string>& params
                   const std::map<std::string, std::string>& result);
};

#endif // __SEARCH_HEADLER_H__
