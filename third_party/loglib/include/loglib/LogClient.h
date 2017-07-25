#ifndef _FLUME_CPP_CLIENT_
#define _FLUME_CPP_CLIENT_
#include "general.h"
#include "FileHelper.h"
#include "SafeQueue.h"
#include <boost/atomic.hpp>
#include "LogClientConfig.h"
class LogClient
{
private:
	string m_sConfigFilePath;
	string m_sCurrentScheFile;
	int m_iCurrentScheLine;
	static boost::mutex m_muInit;
	boost::mutex m_muRun;
	LogClentConfig m_logconf;
	SafeQueue m_sendqueue;

	boost::shared_ptr<TSocket> m_socketFlumeAgent;
	boost::shared_ptr<TFramedTransport> m_transportFlumeAgent;
	boost::shared_ptr<TCompactProtocol> m_protocolFlumeAgent;
	ThriftSourceProtocolClient m_FlumeClient;

	boost::shared_ptr<TSocket> m_socketMonitorFlumeAgent;
	boost::shared_ptr<TFramedTransport> m_transportMonitorFlumeAgent;
	boost::shared_ptr<TCompactProtocol> m_protocolMonitorFlumeAgent;
	ThriftSourceProtocolClient m_MonitorFlumeClient;

	FileHelper m_filehelp;
	boost::thread * m_threadFlume;
	boost::thread m_ThreadScheNoSend;
	boost::thread m_ThreadLetLogForCppRoll;
private:
	static LogClient * m_pInstance;
public:
	static LogClient* GetInstance(const string& configfilepath);
	void SendLog(const string& logtype, const std::map<string,string> & logmap);
	bool isActive();
	void Close();
	~LogClient();
private:
	LogClient(const string& configfile);
	void Init(boost::shared_ptr<TSocket> & socket,boost::shared_ptr<TFramedTransport> & transport
		, boost::shared_ptr<TCompactProtocol> & protocal, ThriftSourceProtocolClient & client);
	int get_filenames(const std::string& dir, std::vector<std::string>& filenames);
	void get_no_send(const std::string& dir, std::vector<std::string>& filenames);
	void sendEvent(ThriftFlumeEvent & levent);
	void DoSend();
	void get_transfer_files(int& first_file_index,vector<string> & files);
	void SendLog(const string& logstring);
	void SendLog(const string& logtype, const string& logstring);
	std::map<string,string> getHead(const string& logtype);
	string ansitoutf_8(const string& ansiString);
	string strReplace(const string& str, const char& replaceStr);
	string getStringFromMap(const string& logtype,const map<string,string>& logmap);
	long long gettimestamp();
	long getLasterFile(const vector<string>& files);
	void ScheNoSend();
	void get_sch_no_send(const string& dir,const int transfered);
	void getHalfSendFile(int & index,string & filename);
	void writeHalfSendFile(const int& index, const string & filename);
	long get_file_created_time(const string& file);
};
#endif
