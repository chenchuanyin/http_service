#ifndef _LOG_CLIENT_CONFIG
#define _LOG_CLIENT_CONFIG
#include "general.h"
using std::string;
struct LogClentConfig
{
	string m_sLocalLogPath;
	string m_sLocalVaLogPath;
	int m_iFlumeConnectTimeOut;
	bool m_bSafeMode;
	string m_sLogLevel;
	string m_sLogType;
	int m_iSchedualIntervalTime;
	int m_iSendQueueSize;
	int m_iSendThreadNumber;
	string m_sFlumeAgentIp;
	int m_iFirstSchedualDelay;
	string m_ssdkLogName;
	string m_slocalLogName;
	string m_slog4cppConfigFile;
	int m_iNoTransferFillPercenty;
	int m_iFlumeAgentPort;
	string m_sMonitorFlumeAgentIp;
	int m_iMonitorFlumeAgentPort;
	string m_sSelfDescId;
	string m_sSelfDescContent;
	LogClentConfig();
	LogClentConfig(const string& configfile);
};
#endif