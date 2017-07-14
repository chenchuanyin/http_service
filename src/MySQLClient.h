#ifndef __DATABASE_CLIENT_H__
#define __DATABASE_CLIENT_H__

#include <Poco/AutoPtr.h>
#include <Poco/Data/SessionPool.h>

class MySQLClient : public Poco::RefCountedObject{
public:
    MySQLClient(std::string host,
                   int port,
                   std::string user,
                   std::string password,
                   std::string db);

    ~MySQLClient();

    bool insert();

    bool select();

private:
    Poco::AutoPtr<Poco::Data::SessionPool> sessionPool_;
};

#endif // __DATABASE_CLIENT_H__
