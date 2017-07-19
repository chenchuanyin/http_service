//
// Created by silver chan on 2017/7/17.
//

#ifndef __SEARCH_SONG_SERVICE_H__
#define __SEARCH_SONG_SERVICE_H__

#include "EnginePool.h"
#include "RedisPool.h"

#include <nlohmann/json.hpp>
#include <Poco/AutoPtr.h>

class SearchSongService {
public:
    SearchSongService(nlohmann::json &param, Poco::AutoPtr<EnginePool> enginePool, Poco::AutoPtr<RedisPool> redisPool);

    EngineRequestReply operator()();

private:
    std::string genRequestParam(const nlohmann::json &param);

private:
    nlohmann::json &param_;
    Poco::AutoPtr<EnginePool> enginePool_;
    Poco::AutoPtr<RedisPool> redisPool_;
};


#endif // __SEARCH_SONG_SERVICE_H__

