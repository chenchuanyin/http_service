#ifndef _loadbalance_config_helper_
#define _loadbalance_config_helper_
#include "general.h"
#include "FileHelper.h"
#include "SafeQueue.h"
#include <boost/atomic.hpp>
#include "LogClientConfig.h"
#include "LoadBalanceItem.h"

class LoadBalanceHelper
{
private:
	static boost::mutex m_muInUseLock;
	LogClentConfig config;
	std::vector<LoadBalanceItem *> clients;
	FileHelper filehelp;
public:
	LoadBalanceHelper(const LogClentConfig& config, const FileHelper & filehelp);
	LoadBalanceHelper();
	bool Init();
	LoadBalanceItem * GetNextClient();
	void SetUnable(LoadBalanceItem * curItem, const int& second);
	void SetUseOver(LoadBalanceItem * curItem);
	~LoadBalanceHelper();
private:
	void SortItems();
};

#endif

