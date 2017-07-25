#include "Log.h"

#include <loglib/LogClient.h>
#include <loglib/LogClientConfig.h>
#include <Poco/Util/LoggingConfigurator.h>

Log::Log() : isInitialized_(false){
#if linux
    client_ = NULL;
#endif
}

Log::~Log() {
}

bool Log::initialize(const Environment &env) {
    logger_ = &Poco::Logger::root();
    Poco::Util::LoggingConfigurator loggingConfigurator;
    loggingConfigurator.configure(env.getConfiguration());
#ifdef __linux__
    client_ = LogClient::GetInstance(Environment::Instance().getString("default.flume.path"));
#endif
    isInitialized_ = true;
    return isInitialized_;
}
