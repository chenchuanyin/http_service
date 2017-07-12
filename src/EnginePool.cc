//
// Created by silver chan on 2017/7/12.
//

#include "EnginePool.h"

#include "Log.h"

EnginePool::EnginePool() {
    pool_.clear();
}

EnginePool::~EnginePool() {
    LOG_DEBUG << "destruct\n";
    for (auto i = pool_.begin(); i != pool_.end(); ++i) {
        delete i->second;
    }
}

bool EnginePool::addClient(std::string ip, int port) {
    return false;
}
