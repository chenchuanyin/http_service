#include "DatabaseClient.h"

#include "Log.h"

#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/MySQL/Connector.h>
#include <Poco/Data/MySQL/MySQLException.h>
#include <iostream>

DatabaseClient::DatabaseClient(std::string host,
                               int port,
                               std::string user,
                               std::string password,
                               std::string db)
{
  try {
    Poco::Data::MySQL::Connector::registerConnector();
    std::string dbConnectStr = Poco::format("host=%s;port=%d;user=%s;password=%s;db=%s;"
                                                    "default-character-set=utf8;compress=true;auto-reconnect=true",
                                            host, port, user, password, db);
    Poco::Data::SessionPool pool("MySQL", dbConnectStr, 1, 4, 5);
    Poco::Data::Session session(pool.get());
    if (session.isConnected()) {
      LOG_INFO << "connected to mysql, host:" << host
               << ", port:" << port << "\n";
    }
    else {
      LOG_ERROR << "connect to mysql(host:" << host
                << ",port:" << port << ") failed!\n";
    }
  }
  catch (Poco::Exception& ex) {
      LOG_ERROR  << ex.displayText() << std::endl;
  }
}

DatabaseClient::~DatabaseClient()
{
}
