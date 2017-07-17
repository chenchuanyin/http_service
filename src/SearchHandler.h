#ifndef __SEARCH_HEADLER_H__
#define __SEARCH_HEADLER_H__

#include <nlohmann/json.hpp>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <map>

class SearchHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response);

    bool checkParam(nlohmann::json &param, nlohmann::json &result);

    void convertParam(nlohmann::json &param);

    bool searchSong(nlohmann::json &param, nlohmann::json &result);

    std::string getSearchRequestJson(const nlohmann::json &param);
};

#endif // __SEARCH_HEADLER_H__
