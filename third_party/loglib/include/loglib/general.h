#include <iostream>
#include <string>
#include <fstream>
#include <map>

#define SIGNED_RIGHT_SHIFT_IS 1
#define ARITHMETIC_RIGHT_SHIFT 1

#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include <boost/thread/shared_mutex.hpp> 
#include <boost/thread.hpp>

#include "general_fun.h"
#include "../gen-cpp/flume_constants.h"
#include "../gen-cpp/ThriftSourceProtocol.h"
#include "../gen-cpp/flume_types.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using std::string;