//
// Created by silver chan on 2017/7/12.
//

#ifndef __ENGINE_CLIENT_H__
#define __ENGINE_CLIENT_H__

#include <Poco/Timestamp.h>

#include <string>

class EngineClient {
public:
    EngineClient(std::string ip, int port);

    ~EngineClient();

    void setStatus(bool status) { status_ = status; }

    bool status() const { return status_; }

    bool timeLimit();

    void updateTime();

    std::string toString() const;

private:
    std::string ip_;
    int port_;
    bool status_;
    Poco::Timestamp timestamp_;

};


#endif // __ENGINE_CLIENT_H__
