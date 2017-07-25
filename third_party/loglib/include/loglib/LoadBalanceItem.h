#ifndef _loadbalance_item_
#define _loadbalance_item_

#include "general.h"
#include "FileHelper.h"
#include "SafeQueue.h"
#include <boost/atomic.hpp>
#include "LogClientConfig.h"
#include "FileHelper.h"

class LoadBalanceItem
{
private:
	std::string m_sHostIp;
	FileHelper m_ffileHelper;
	int m_iHostPort;
	bool m_bIsInUse;
	int m_iHostTimeOut;
	long long m_llResumeTime;
	ThriftSourceProtocolClient m_tThriftClient;
	boost::shared_ptr<TSocket> m_socketFlumeAgent;
	boost::shared_ptr<TFramedTransport> m_transportFlumeAgent;
	boost::shared_ptr<TCompactProtocol> m_protocolFlumeAgent;
public:
	std::string& GetHostIp();
	int GetHostPort();
	void Init();
	void Close();
	int GetHostTimeOut();
	ThriftSourceProtocolClient GetClient();
	void Send(ThriftFlumeEvent & curEvent);
	long long GetResumeTime();
	void SetHostIP(const std::string& ip);
	bool GetIsInUse();
	void SetIsInUse(const bool& inuse);
	void SetHostPort(const int& port);
	void SetHostTimeOut(const int & timeout);
	void SetResumeTime(const int & second);
	LoadBalanceItem(const std::string& ip,const int& port,const int& timeout,const long long& resumtime,const FileHelper& filehelper);
	void SetUnable(const int & second);
	~LoadBalanceItem();
	LoadBalanceItem();
};

#endif