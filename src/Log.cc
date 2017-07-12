#include "Log.h"

#include <Poco/Util/LoggingConfigurator.h>

Log::Log() : isInitialized_(false) {
}

Log::~Log() {
}

bool Log::initialize(const Environment &env) {
    logger_ = &Poco::Logger::root();
    Poco::Util::LoggingConfigurator loggingConfigurator;
    loggingConfigurator.configure(env.getConfiguration());
    isInitialized_ = true;
    return isInitialized_;
}
