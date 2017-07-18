//
// Created by silver chan on 2017/7/13.
//

#ifndef __ENGINE_REQUEST_REPLY_H__
#define __ENGINE_REQUEST_REPLY_H__

#include <nlohmann/json.hpp>

#include <map>
#include <string>

struct EngineRequestReply {
    nlohmann::json engineReply;
    std::string engineAddress;
    nlohmann::json error;
    std::map<std::string, int> statistics;
};


#endif // __ENGINE_REQUEST_REPLY_H__
