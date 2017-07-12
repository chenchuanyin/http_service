//
// Created by silver chan on 2017/7/12.
//

#ifndef __ENGINE_POOL_H__
#define __ENGINE_POOL_H__

#include <map>
#include <string>

class EngineClient;

class EnginePool {
public:
    EnginePool();
    ~EnginePool();

    bool addClient(std::string ip, int port);

private:
    std::map<std::string, EngineClient *> pool_;
};


#endif // __ENGINE_POOL_H__
