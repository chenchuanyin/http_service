//
// Created by silver chan on 2017/7/12.
//

#include "Application.h"

#include "Environment.h"
#include "Log.h"
#include "RequestHandlerFactory.h"

#include <cstdlib>

Application::Application(const std::string& configFile)
        : configFile_(configFile), httpServer_(NULL){
}

Application::~Application() {
    if (httpServer_) {
        httpServer_->stop();
        delete httpServer_;
        httpServer_ = NULL;
    }
}

bool Application::initialize() {
    if (initConfig()) {
        fprintf(stdout, "init config completed\n");
    } else {
        fprintf(stderr, "init config failed!\n");
        return false;
    }
    if (initLog()) {
        LOG_INFO << "init log completed\n";
    } else {
        fprintf(stderr, "init log failed!\n");
        return false;
    }
    if (initService()) {
        LOG_INFO << "init service completed\n";
    } else {
        LOG_ERROR << "application init failed\n";
        return false;
    }
    return true;
}

bool Application::unInitialize() {
    return true;
}

bool Application::initConfig() {
    bool result = Environment::Instance().initialize(configFile_);
    return result;
}

bool Application::initLog() {
    bool result = Log::Instance().initialize(Environment::Instance());
    return result;
}

bool Application::initService() {
    initRedis();
    initHttpServer();
    return true;
}

bool Application::initHttpServer() {
    LOG_INFO << "start to init http server.\n";
    try {
        int port = Environment::Instance().getInt("http.port");
        int threads = Environment::Instance().getInt("http.threads");
        Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams;
        params->setMaxQueued(100);
        params->setMaxThreads(threads);
        Poco::Net::ServerSocket svs(port);
        httpServer_ = new Poco::Net::HTTPServer(new RequestHandlerFactory(), svs, params);
        LOG_INFO << "init http server ok! port:" << port << "\n";
        return true;
    }
    catch (Poco::Exception& ex) {
        LOG_ERROR << ex.displayText() << "\n";
    }
}

bool Application::initRedis() {
    redisPool_ = new RedisPool();
    std::string redisCache = Environment::Instance().getString("redis.redis_cache");
    addRedis(redisCache);
    std::string kuyinHot = Environment::Instance().getString("redis.kuyin_hot");
    addRedis(kuyinHot);
    std::string redisSoa = Environment::Instance().getString("redis.redis_soa");
    addRedis(redisSoa);
    return true;
}

bool Application::addRedis(const std::string& redisAddress) {
    Poco::StringTokenizer splitterTokenizer(redisAddress, ":");
    if (splitterTokenizer.count() == 2) {
        return redisPool_->addClient(splitterTokenizer[0], std::atoi(splitterTokenizer[1].c_str()));
    }
    else if (splitterTokenizer.count() == 3) {
        return redisPool_->addClient(splitterTokenizer[0], std::atoi(splitterTokenizer[1].c_str()), splitterTokenizer[2]);
    }
    else {
        LOG_ERROR << "error formmat,skip! content:" << redisAddress << "\n";
        return false;
    }
}

void Application::run() {
    httpServer_->start();
}


