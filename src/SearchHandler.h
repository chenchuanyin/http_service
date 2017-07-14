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

    bool checkParams(std::map<std::string, std::string> &params,
                     std::map<std::string, std::string> &result);

    void convertParams(std::map<std::string, std::string> &params);

    bool searchSong(std::map<std::string, std::string> &params,
                    std::map<std::string, std::string> &result);

    std::string getSearchRequestJson(std::map<std::string, std::string> &params);
};

#endif // __SEARCH_HEADLER_H__
