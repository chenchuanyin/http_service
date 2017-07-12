//
// Created by silver chan on 2017/7/12.
//

#ifndef __APPLICATION_H__
#define __APPLICATION_H__


#include <Poco/Net/HTTPServer.h>

class Application {
public:
    Application(const std::string& configFile);
    ~Application();
    bool initialize();
    void run();
    bool unInitialize();

private:
    bool initConfig();
    bool initLog();
    bool initService();
    Application& operator=(const Application&);
    Application(const Application&);

private:
    std::string configFile_;
    Poco::Net::HTTPServer *httpServer_;

};


#endif // __APPLICATION_H__
