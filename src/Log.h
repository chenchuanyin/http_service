#ifndef __LOG_H__
#define __LOG_H__

#include "Environment.h"

#include <loglib/LogClient.h>
#include <loglib/LogClientConfig.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/LocalDateTime.h>
#include <libgen.h>
#include <stdio.h>

#ifdef __linux__
class LogClient;
#endif

class Log {
public:
    static Log &Instance() {
        static Log log;
        return log;
    }

    bool initialize(const Environment &env);

    Poco::Logger &getLogger() const { return *logger_; }

    void sendLog(const std::string &message);

    std::string getLogPrefix(const std::string &logType,
                             const std::string &fileName,
                             const int lineNumber,
                             const std::string &funcName);

protected:
    Log();

    ~Log();

private:
    Log &operator=(const Log &);

    Log(const Log &);

private:
    Poco::Logger *logger_;
    bool isInitialized_;
#ifdef __linux__
    LogClient *client_;
#endif
};

#define LOG_DEBUG(fmt,args...) if (Log::Instance().getLogger().debug()) \
{ \
    std::string logMessage =  Log::Instance().getLogPrefix("Debug",basename(__FILE__), __LINE__, __func__); \
    char buffer[10240]; \
    sprintf(buffer, fmt, ##args); \
    logMessage.append(buffer); \
    Log::Instance().getLogger().debug(logMessage); \
    Log::Instance().sendLog(logMessage); \
}


#define LOG_INFO(fmt,args...) if (Log::Instance().getLogger().information()) \
{ \
    std::string logMessage =  Log::Instance().getLogPrefix("Info ",basename(__FILE__), __LINE__, __func__); \
    char buffer[10240]; \
    sprintf(buffer, fmt, ##args); \
    logMessage.append(buffer); \
    Log::Instance().getLogger().information(logMessage); \
    Log::Instance().sendLog(logMessage); \
}

#define LOG_WARN(fmt,args...) if (Log::Instance().getLogger().warning()) \
{ \
    std::string logMessage =  Log::Instance().getLogPrefix("Warn ",basename(__FILE__), __LINE__, __func__); \
    char buffer[10240]; \
    sprintf(buffer, fmt, ##args); \
    logMessage.append(buffer); \
    Log::Instance().getLogger().warning(logMessage); \
    Log::Instance().sendLog(logMessage); \
}

#define LOG_ERROR(fmt,args...) if (Log::Instance().getLogger().error()) \
{ \
    std::string logMessage =  Log::Instance().getLogPrefix("Error",basename(__FILE__), __LINE__, __func__); \
    char buffer[10240]; \
    sprintf(buffer, fmt, ##args); \
    logMessage.append(buffer); \
    Log::Instance().getLogger().error(logMessage); \
    Log::Instance().sendLog(logMessage); \
}

#endif // __LOG_H__
