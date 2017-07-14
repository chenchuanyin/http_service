#include "MySQLClient.h"

#include "Log.h"

#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <iostream>

MySQLClient::MySQLClient(std::string host,
                         int port,
                         std::string user,
                         std::string password,
                         std::string db) {
    try {
        Poco::Data::MySQL::Connector::registerConnector();
        std::string dbConnectStr = Poco::format("host=%s;port=%d;user=%s;password=%s;db=%s;"
                                                        "default-character-set=utf8;compress=true;auto-reconnect=true",
                                                host, port, user, password, db);
        sessionPool_ = new Poco::Data::SessionPool("MySQL", dbConnectStr);
        Poco::Data::Session session(sessionPool_->get());
        if (session.isConnected()) {
            LOG_INFO << "connected to mysql, host:" << host
                     << ", port:" << port << "\n";
        } else {
            LOG_ERROR << "connect to mysql(host:" << host
                      << ",port:" << port << ") failed!\n";
        }
    }
    catch (Poco::Exception &ex) {
        LOG_ERROR << ex.displayText() << std::endl;
    }
}

MySQLClient::~MySQLClient() {

}
