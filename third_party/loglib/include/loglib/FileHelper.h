#ifndef LOG4FILEHELPER
#define LOG4FILEHELPER

#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/layout.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/loggingmacros.h>
#include <string>
#include <boost/thread/shared_mutex.hpp> 
#include <boost/thread.hpp>
class FileHelper
{
private:
	log4cplus::Logger m_logSdkLogger;
	log4cplus::Logger m_logLocalFile;
	std::string m_strSdkLogDirectory;
	std::string m_strLocalLogDirectory;
	std::string m_strSdkLogLevel;
public:
	FileHelper(){};
	~FileHelper(){};
	FileHelper(const std::string& log4cpp_config_file, const std::string& strSdkLogDirectory, const std::string& strLocalLogDirectory, const std::string& strSdkLogLevel,const std::string& sdkLogName, const std::string& localLogName);
	void WriteLog(const std::string& strCurrentSdkLogLevel, const std::string& strSdkLog);
	void WriteString(const std::string& strLocalLog);
};

#endif