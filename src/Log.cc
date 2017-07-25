#include "Log.h"

#include <loglib/LogClient.h>
#include <loglib/LogClientConfig.h>
#include <Poco/Util/LoggingConfigurator.h>

Log::Log() : isInitialized_(false) {
#ifdef __linux__
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

std::string Log::getLogPrefix(const std::string &logType,
                              const std::string &fileName,
                              const int lineNumber,
                              const std::string &funcName) {
    std::stringstream ss;
    Poco::LocalDateTime now;
    ss << "[" << now.year() << "-" << now.month() << "-" << now.day() << " "
       << now.hour() << ":" << now.minute() << ":" << now.second() << "." << now.millisecond()
       << " " << logType << " "
       << fileName << ":" << lineNumber << ":" << funcName << "()]";
    return ss.str();
}

void Log::sendLog(const std::string &message) {
#ifdef __linux__
    try {
        std::map<std::string, std::string> map;
        map["message"] = message;
        client_->sendLog("MusicSearch", map);
    } catch (std::exception &ex) {
        std::cerr << ex.what() << std::endl;
    }
#endif
//    fprintf(stderr, "log:%s\n", message.c_str());
}
