#include "Environment.h"
#include "Log.h"
#include "RequestHandlerFactory.h"

#include <Poco/Channel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/DNS.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/TCPServer.h>
#include <Poco/Net/TCPServerConnectionFactory.h>
#include <Poco/PatternFormatter.h>
#include <Poco/SplitterChannel.h>
#include <Poco/StreamCopier.h>


#include <string.h>

bool InitConfig(const std::string& filename)
{
  Environment::Instance().initialize(filename);
  return true;
}

void InitLogger() {
  Log::Instance().initialize(Environment::Instance());
  LOG_INFO << "init config ok!\n";
}

void InitHttpServer() {
  LOG_ERROR << "start to init http server.\n";
  int port = Environment::Instance().getInt("default.port");
  Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams;
  params->setMaxQueued(100);
  params->setMaxThreads(16);
  Poco::Net::ServerSocket svs(port);
  Poco::Net::HTTPServer server(new RequestHandlerFactory(), svs, params);
  server.start();
  LOG_WARN << "init http server ok! port:" << port << "\n";
  getchar();
}

int main(void) {
  InitConfig("default.conf");
  InitLogger();
  InitHttpServer();
  return 0;
}
