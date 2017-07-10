#include "RequestHandlerFactory.h"

#include <Poco/AutoPtr.h>
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
#include <Poco/Util/IniFileConfiguration.h>

#include <string.h>

class RootHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest &request,
                       Poco::Net::HTTPServerResponse &response) {
        Poco::Logger::root().information(
            "handleRequest, host:%s, method:%s, uri:%s", request.getHost(),
            request.getMethod(), request.getURI());
        response.sendBuffer("hello world", strlen("hello world"));
    }
};

Poco::AutoPtr<Poco::Util::IniFileConfiguration> InitConf(std::string filename) {
    Poco::AutoPtr<Poco::Util::IniFileConfiguration> conf_ptr(
        new Poco::Util::IniFileConfiguration(filename));
    return conf_ptr;
}

void InitLogger(Poco::AutoPtr<Poco::Util::IniFileConfiguration> config) {
    std::string logPath = config->getString("log.path", "log/log.txt");
    std::string pattern =
        config->getString("log.pattern", "%Y %m %d %H:%M:%S %s(%l): %t");
    Poco::AutoPtr<Poco::FileChannel> fileChannelPtr(new Poco::FileChannel);
    fileChannelPtr->setProperty("path", logPath); //指定日志路径及文件名
    Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(
        new Poco::PatternFormatter());
    patternFormatter->setProperty("pattern", pattern); //每条日志时间
    Poco::AutoPtr<Poco::Channel> formattingFileChannelPtr =
        new Poco::FormattingChannel(patternFormatter,
                                    fileChannelPtr); //初始化　Channel
    Poco::AutoPtr<Poco::SplitterChannel> splitterChannelPtr(
        new Poco::SplitterChannel);
    Poco::AutoPtr<Poco::ConsoleChannel> consoleChannelPtr(
        new Poco::ConsoleChannel);
    Poco::AutoPtr<Poco::Channel> formattingConsoleChannelPtr =
        new Poco::FormattingChannel(patternFormatter, consoleChannelPtr);
    splitterChannelPtr->addChannel(formattingConsoleChannelPtr);
    splitterChannelPtr->addChannel(formattingFileChannelPtr);
    Poco::Logger::root().setChannel(splitterChannelPtr);
    Poco::Logger::root().information("init config ok!");
}

void InitHttpServer(Poco::AutoPtr<Poco::Util::IniFileConfiguration> config) {
    Poco::Logger::root().information("start to init http server.");
    int port = config->getInt("default.port", 8888);
    Poco::Net::HTTPServerParams *params = new Poco::Net::HTTPServerParams;
    params->setMaxQueued(100);
    params->setMaxThreads(16);
    Poco::Net::ServerSocket svs(port);
    Poco::Net::HTTPServer server(new RequestHandlerFactory(), svs, params);
    server.start();
    Poco::Logger::root().information("init http server ok! port:%d", port);
    getchar();
}

int main(void) {
    Poco::AutoPtr<Poco::Util::IniFileConfiguration> config =
        InitConf("default.conf");
    InitLogger(config);
    InitHttpServer(config);
    getchar();
    return 0;
}
