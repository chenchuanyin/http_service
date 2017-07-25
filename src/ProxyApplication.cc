//
// Created by silver chan on 2017/7/12.
//

#include "ProxyApplication.h"

#include "Environment.h"
#include "Log.h"
#include "HttpRequestHandlerFactory.h"
#include <Poco/StringTokenizer.h>
#include <Poco/Util/HelpFormatter.h>

ProxyApplication::ProxyApplication(const std::string &configFile)
        : configFile_(configFile), httpServer_(NULL) {
}

ProxyApplication::~ProxyApplication() {
    if (httpServer_) {
        httpServer_->stop();
        delete httpServer_;
        httpServer_ = NULL;
    }
}


bool ProxyApplication::initConfig() {
    bool result = Environment::Instance().initialize(configFile_);
    return result;
}

bool ProxyApplication::initLog() {
    bool result = Log::Instance().initialize(Environment::Instance());
    return result;
}

bool ProxyApplication::initService() {
    initRedisPool();
    initEnginePool();
    initHttpServer();
    return true;
}

bool ProxyApplication::initHttpServer() {
    LOG_DEBUG("start to init http server")
    try {
        int port = Environment::Instance().getInt("http.port");
        int threads = Environment::Instance().getInt("http.threads");
        int maxQueue = Environment::Instance().getInt("http.max_connection_limit");
        Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams;
        params->setMaxQueued(maxQueue);
        params->setMaxThreads(threads);
        Poco::Net::ServerSocket svs(port);
        httpServer_ = new Poco::Net::HTTPServer(new HttpRequestHandlerFactory(enginePool_, redisPool_), svs, params);
        LOG_INFO("init http server ok! port:%d", port);
        return true;
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR("%s", ex.displayText().c_str());
    }
}

bool ProxyApplication::initRedisPool() {
    redisPool_ = new RedisPool();
    std::string redisCache = Environment::Instance().getString("redis.redis_cache");
    addRedis(redisCache);
    std::string kuyinHot = Environment::Instance().getString("redis.kuyin_hot");
    addRedis(kuyinHot);
    std::string redisSoa = Environment::Instance().getString("redis.redis_soa");
    addRedis(redisSoa);
    return true;
}

bool ProxyApplication::addRedis(const std::string &redisAddress) {
    Poco::StringTokenizer splitterTokenizer(redisAddress, ":");
    if (splitterTokenizer.count() == 2) {
        return redisPool_->addClient(splitterTokenizer[0], std::atoi(splitterTokenizer[1].c_str()));
    } else if (splitterTokenizer.count() == 3) {
        return redisPool_->addClient(splitterTokenizer[0], std::atoi(splitterTokenizer[1].c_str()),
                                     splitterTokenizer[2]);
    } else {
        LOG_ERROR("error formmat,skip! content:%s", redisAddress.c_str());
        return false;
    }
}

bool ProxyApplication::initEnginePool() {
    enginePool_ = new EnginePool();
    std::string searchEngineAddresses = Environment::Instance().getString("engine.address");
    LOG_INFO("search engine:%s", searchEngineAddresses.c_str());
    Poco::StringTokenizer searchCountTokenizer(searchEngineAddresses, ",");
    for (int i = 0; i < searchCountTokenizer.count(); ++i) {
        Poco::StringTokenizer splitTokenizer(searchCountTokenizer[i], ":");
        if (splitTokenizer.count() == 2) {
            enginePool_->addClient(splitTokenizer[0], std::atoi(splitTokenizer[1].c_str()), true);
        } else {
            LOG_ERROR("wrong format for engine address, content:%s", searchCountTokenizer[i].c_str());
        }
    }
    std::string suggestEngineAddresses = Environment::Instance().getString("suggest.address");
    LOG_INFO("suggest engine:%s", suggestEngineAddresses.c_str());
    Poco::StringTokenizer suggestCountTokenizer(suggestEngineAddresses, ",");
    for (int i = 0; i < suggestCountTokenizer.count(); ++i) {
        Poco::StringTokenizer splitTokenizer(suggestCountTokenizer[i], ":");
        if (splitTokenizer.count() == 2) {
            enginePool_->addClient(splitTokenizer[0], std::atoi(splitTokenizer[1].c_str()), false);
        } else {
            LOG_ERROR("wrong format for engine address, content:%s", suggestCountTokenizer[i].c_str());
        }
    }
}

void ProxyApplication::uninit() {
}

void ProxyApplication::init() {
    if (initConfig()) {
        fprintf(stdout, "init config completed\n");
    } else {
        fprintf(stderr, "init config failed!\n");
        throw Poco::Exception("init config failed");
    }
    if (initLog()) {
        LOG_INFO("init log completed");
    } else {
        fprintf(stderr, "init log failed!\n");
        throw Poco::Exception("init log failed");
    }
    if (initService()) {
        LOG_INFO("init service completed");
    } else {
        LOG_ERROR("init service failed");
        throw Poco::Exception("init service failed");
    }
}


int ProxyApplication::main(const std::vector<std::string> &args) {
    httpServer_->start();
    LOG_INFO("server start...");
    waitForTerminationRequest();
    httpServer_->stop();
    LOG_INFO("server stop...");
    return Poco::Util::Application::EXIT_OK;
}

