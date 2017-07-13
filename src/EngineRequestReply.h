//
// Created by silver chan on 2017/7/13.
//

#ifndef __ENGINE_REQUEST_REPLY_H__
#define __ENGINE_REQUEST_REPLY_H__

#include <map>
#include <string>

struct EngineRequestReply {
    std::string engineReply;
    std::string engineAddress;
    std::map<std::string, std::string> errors;
    std::map<std::string, int> statistics;
};


#endif // __ENGINE_REQUEST_REPLY_H__
