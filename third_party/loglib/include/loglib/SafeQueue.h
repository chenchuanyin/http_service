#ifndef _FLUME_CPP_SAFEQUEUE_
#define _FLUME_CPP_SAFEQUEUE_
#include <queue>
#include "general.h"
#include <boost/thread/recursive_mutex.hpp>
class SafeQueue
{
private:
	std::queue<ThriftFlumeEvent> m_qEvent;
	size_t m_sMaxSize;
	boost::mutex m_mut;
public:
	size_t length();
	ThriftFlumeEvent Dequeue();
	bool Enqueue(ThriftFlumeEvent element);
	void setSize(int size);
	SafeQueue();
	~SafeQueue(void);
};
#endif