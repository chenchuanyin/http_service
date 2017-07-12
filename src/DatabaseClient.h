#ifndef __DATABASE_CLIENT_H__
#define __DATABASE_CLIENT_H__

#include <string>

class DatabaseClient {
 public:
  DatabaseClient(std::string host,
                 int port,
                 std::string user,
                 std::string password,
                 std::string db);
  ~DatabaseClient();

  bool insert();
  bool select();
};
#endif // __DATABASE_CLIENT_H__
