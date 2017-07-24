#ifndef __LOG_H__
#define __LOG_H__

#include "Environment.h"

#include <loglib/LogClient.h>
#include <loglib/LogClientConfig.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <libgen.h>

#if linux
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

protected:
    Log();

    ~Log();

private:
    Log &operator=(const Log &);

    Log(const Log &);

private:
    Poco::Logger *logger_;
    bool isInitialized_;
#if linux
    LogClient *client_;
#endif
};

#define LOG_DEBUG if (Log::Instance().getLogger().debug()) \
    Poco::LogStream(Log::Instance().getLogger()).debug() << "[" << basename(__FILE__) << ":" << __func__ << "()-" << __LINE__ << "]:"
#define LOG_INFO if (Log::Instance().getLogger().information()) \
    Poco::LogStream(Log::Instance().getLogger()).information() << "[" << basename(__FILE__) << ":" << __func__ << "()-" << __LINE__ << "]:"
#define LOG_WARN if (Log::Instance().getLogger().warning()) \
    Poco::LogStream(Log::Instance().getLogger()).warning() << "[" << basename(__FILE__) << ":" << __func__ << "()-" << __LINE__ << "]:"
#define LOG_ERROR if (Log::Instance().getLogger().error()) \
    Poco::LogStream(Log::Instance().getLogger()).error() << "[" << basename(__FILE__) << ":" << __func__ << "()-" << __LINE__ << "]:"
#define LOG_FATAL if (Log::Instance().getLogger().fatal()) \
    Poco::LogStream(Log::Instance().getLogger()).fatal() << "[" << basename(__FILE__) << ":" << __func__ << "()-" << __LINE__ << "]:"

#endif // __LOG_H__
